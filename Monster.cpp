#include "Monster.hpp"

#include <cfloat>
#include <cmath>
#include <vector>
#include <cstdlib>
#include <ctime>
#include <iostream>

using namespace sf;
using namespace std;

static const float MONSTER_SIZE = 1.6f * REF_SIZE;

static const int MONSTER_VALUE = 100;

static const float MONSTER_SPEED = 0.95f * REF_SPEED;
static const float MONSTER_PANIC_SPEED = 0.77f * REF_SPEED;
static const float MONSTER_SLOW_SPEED = 0.55f * REF_SPEED;
static const float MONSTER_KILLED_SPEED = 1.3f * REF_SPEED;

static const sf::Color MONSTER_BEGIN_PANIC_MODE_COLOR(sf::Color::Blue);
static const sf::Color MONSTER_END_PANIC_MODE_COLOR(sf::Color::White);
static const sf::Color MONSTER_EATEN_COLOR(0, 0, 255, 127);

static const sf::Time MONSTER_END_PANIC_MODE_TIMER(sf::seconds(0.5f));

static const Coordinates MONSTER_HOUSE_ENTER_COORDINATES = {14.f * CELL_SIZE, 14.5f * CELL_SIZE};
static const Coordinates MONSTER_HOUSE_COORDINATES = {14.f * CELL_SIZE, 17.5f * CELL_SIZE};

static const int MONSTER_HOUSE_ENTER1[] = {13, 14};
static const int MONSTER_HOUSE_ENTER2[] = {14, 14};



//
// Protected functions
//

Monster::Monster(Coordinates& _position, const Tile& _homeTarget, const Grid& _grid, const Pacman* _pacman, const Color& _monsterColor) : rectangleShape(Vector2f(MONSTER_SIZE, MONSTER_SIZE)){
    position = _position;
    homeTarget = _homeTarget;
    grid = _grid;
    pacman = _pacman;
    monsterColor = _monsterColor;
    behaviour = BEHAVIOUR::SCATTER_MODE;
    direction = COORDINATES_DIRECTION.at(DIRECTION::LEFT);
    rootingDirection = direction;
    
    infectionState = INFECTION_STATE::SAFE;
    
    target = grid(MONSTER_HOUSE_ENTER1);
    tile = grid.getTileFromPosition(position);
    
    speed = MONSTER_SPEED;
    value = MONSTER_VALUE;
    
    rectangleShape.setOrigin(MONSTER_SIZE / 2, MONSTER_SIZE / 2);
    rectangleShape.setPosition(position);
    rectangleColor = monsterColor;
    rectangleShape.setFillColor(rectangleColor);
    
    srand(time(NULL));
}

void Monster::move(Time elapsed){
    if(tile.getType() == TILE_TYPE::SLOW){
        isDirectionFixedOnSlowTile = true;
        speed = MONSTER_SLOW_SPEED;
    }else{
        isDirectionFixedOnSlowTile = false;
        if(behaviour == BEHAVIOUR::PANIC_MODE)
            speed = MONSTER_PANIC_SPEED;
        else
            speed = MONSTER_SPEED;
    }
    
    if(isDead())
        speed = MONSTER_KILLED_SPEED;
    
    updatePosition(elapsed);
    rectangleShape.setPosition(position);
}

Coordinates Monster::computeRootingDirection(){
    int tileIndexes[2];
    Grid::getTileIndexesFromPosition(tile.getPosition(), tileIndexes); // get the tile indexes of the tile where the monster is
    
    // Get the next tile in the direction of movement
    tileIndexes[0] += direction.x;
    tileIndexes[1] += direction.y;
    
    if(behaviour != BEHAVIOUR::PANIC_MODE || isDead()){
        float minDistance = FLT_MAX;
        Coordinates rootingDir;
        for(int i = 0; i < NB_DIRECTIONS; i++){
            Coordinates nextDirection = COORDINATES_DIRECTION.at(static_cast<DIRECTION>(i));
            if((canTurn180
               || nextDirection != direction * -1)
               && (isDead()
                   || !(grid(tileIndexes).getType() == TILE_TYPE::ESCAPE
                           && nextDirection == COORDINATES_DIRECTION.at(DIRECTION::UP)))){ // if the next direction is not leading back to the current monster tile (Hence, no 'U'-turn)
                if(nextDirection == direction * -1)
                    canTurn180 = false;
                Tile nextNextTile = grid(tileIndexes[0] + nextDirection.x, tileIndexes[1] + nextDirection.y); // compute the next next tile in the direction considered
                if(!nextNextTile.hasWall()){ // if the tile has no wall
                    float distance = nextNextTile.getPosition().getDistanceToCoordinates(target.getPosition());
                    if(distance <= minDistance){ // Keep the smallest distance from the next next tile to the target, and the corresponding direction to go from the next tile to the next next tile
                        minDistance = distance;
                        rootingDir = nextDirection;
                    }
                }
            }
        }
        return rootingDir;
        
    }else{
        
        vector<Coordinates> nextDirections;
        nextDirections.reserve(NB_DIRECTIONS);
        for(int i = 0; i < NB_DIRECTIONS; i++){
            Coordinates nextDirection = COORDINATES_DIRECTION.at(static_cast<DIRECTION>(i));
            if(nextDirection != direction * -1.f){ // if the next direction is not leading the monster backwards (Hence, no 'U'-turn), except if it can do a U turn
                Tile nextNextTile = grid(tileIndexes[0] + nextDirection.x, tileIndexes[1] + nextDirection.y);
                if(!nextNextTile.hasWall()){ // if the next tile has no wall
                    nextDirections.push_back(nextDirection);
                }
            }
        }
        
        return nextDirections[rand() % nextDirections.size()];
    }
}

void Monster::flee(Time panicTimer){
    if(panicTimer > Time::Zero){
        if(panicTimer > MONSTER_END_PANIC_MODE_TIMER)
            rectangleColor = MONSTER_BEGIN_PANIC_MODE_COLOR;
        else
            rectangleColor = MONSTER_END_PANIC_MODE_COLOR;
    }
}

void Monster::regenerate(){
    isDead_ = false;
    isImmune_ = false;
    isMonsterHouseMove = true;
    speed = MONSTER_SPEED;
    rectangleColor = monsterColor;
}



//
// Public Functions
//

int Monster::kill(){
    if(isDead_)
        throw domain_error("Monster already killed: can't be killed again");
    isDead_ = true;
    isImmune_ = true;
    infectionState = INFECTION_STATE::SAFE;
    infectionTimer = Time::Zero;
    rectangleColor = MONSTER_EATEN_COLOR;
    canTurn180 = true;
    return value;
}

void Monster::update(sf::Time elapsed, sf::Time behaviourTimer, BEHAVIOUR _behaviour){
    updateVirus(elapsed);
    if(elapsed == Time::Zero){
        behaviour = _behaviour;
    }
        
    
    bool hasAlreadyUpdatedDirection = false;   
    if(isDead()){ // if the monster is dead
        Coordinates monsterHouseEnter1 = grid(MONSTER_HOUSE_ENTER1).getPosition();
        Coordinates monsterHouseEnter2 = grid(MONSTER_HOUSE_ENTER2).getPosition();
        if(position.getDistanceToCoordinates(monsterHouseEnter1) < position.getDistanceToCoordinates(monsterHouseEnter2))
            target = grid(MONSTER_HOUSE_ENTER1);
        else
            target = grid(MONSTER_HOUSE_ENTER2);
    }
        
    if(isDead() && !isMonsterHouseMove && tile == target){ // if the monster is dead AND it is not yet a monster house move AND the target tile is not equal to the current tile
        isMonsterHouseMove = true;
    }else if(isDead() && isMonsterHouseMove){ // if the monster is dead AND it is a monster house move
        if(!isInFrontOfMonsterHouse){ // if the monster is not just in front of the monster house door
            direction = Coordinates::getDirectionBtwPositions(position, MONSTER_HOUSE_ENTER_COORDINATES);
            move(elapsed);
            if(Coordinates::getDirectionBtwPositions(position, MONSTER_HOUSE_ENTER_COORDINATES) != direction){ // if the monster has crossed the monster house entrance position
                position = MONSTER_HOUSE_ENTER_COORDINATES;
                isInFrontOfMonsterHouse = true;
                //return;
            }
        }else{
            direction = Coordinates::getDirectionBtwPositions(position, MONSTER_HOUSE_COORDINATES);
            move(elapsed);
            if(Coordinates::getDirectionBtwPositions(position, MONSTER_HOUSE_COORDINATES) != direction){ // if the monster has crossed the monster house position
                position = MONSTER_HOUSE_COORDINATES;
                direction = COORDINATES_DIRECTION.at(DIRECTION::UP);
                isInFrontOfMonsterHouse = false;
                regenerate(); // regenerate the monster
                //return;
            }
        }
    }else{
        if(!isDead() && isMonsterHouseMove){ // if the monster is alive AND it is a monster house move
            if(isMovingToHouseCenter){ // if the monster is moving to the monster house center
                direction = Coordinates::getDirectionBtwPositions(position, MONSTER_HOUSE_COORDINATES);
                move(elapsed);
                if(Coordinates::getDirectionBtwPositions(position, MONSTER_HOUSE_COORDINATES) != direction){ // if the monster has crossed the monster house coordinates
                    isMovingToHouseCenter = false;
                    position = MONSTER_HOUSE_COORDINATES;
                    //return;
                }
            }else{
                direction = Coordinates::getDirectionBtwPositions(position, MONSTER_HOUSE_ENTER_COORDINATES);
                move(elapsed);
                if(Coordinates::getDirectionBtwPositions(position, MONSTER_HOUSE_ENTER_COORDINATES) != direction){ // if the monster has crossed the monster house entrance coordinates
                    position = MONSTER_HOUSE_ENTER_COORDINATES;
                    direction = COORDINATES_DIRECTION.at(DIRECTION::LEFT); // set the direction at the exit of the monster house to LEFT
                    rootingDirection = direction;
                    isMonsterHouseMove = false;
                    behaviour = _behaviour;
                    return;
                }
            }
        }else if(!isDead()){ // if the monster is alive
            if(behaviour != _behaviour){ // if there is a change of behaviour
                if(behaviour != BEHAVIOUR::PANIC_MODE){ // if the initial behaviour is not PANIC_MODE
                    behaviour = _behaviour;
                    
                    // Reverse travel direction
                    direction = direction * -1.f;
                    rootingDirection = previousDirection * -1.f;
                    if(_behaviour != BEHAVIOUR::PANIC_MODE){ // if the new behaviour is not PANIC_MODE
                        nextTarget = computeNextTarget();
                    }
                    
                    if(!isDirectionFixedOnSlowTile){ // if the direction is not fixed because the monster is not on a slow tile
                        Coordinates directionToTile = Coordinates::getDirectionBtwPositions(position, tile.getPosition());
                        if(direction != directionToTile) // if the monster has crossed the middle position of the tile in its direction of travel
                            canTurn = false;
                        else
                            canTurn = true;
                        
                        hasAlreadyUpdatedDirection = true; // can not update direction anymore this frame as it has just been done
                    }
                }else{
                    behaviour = _behaviour;
                    rectangleColor = monsterColor;
                }
            }
            
            if(behaviour == BEHAVIOUR::PANIC_MODE) // if the monster is in PANIC_MODE
                flee(behaviourTimer);
        }
    
        if(!isDirectionFixedOnSlowTile && !hasAlreadyUpdatedDirection){
            Coordinates directionToTile = Coordinates::getDirectionBtwPositions(position, tile.getPosition());
            
            if(canTurn && direction != directionToTile){ // if the monster is allowed to turn AND it has crossed the middle position of the tile it is on
                previousDirection = direction;
                position = tile.getPosition();
                direction = rootingDirection;
                canTurn = false;
                rootingDirection = computeRootingDirection();
            }else if(direction == directionToTile){ // if the monster has not yet crossed the middle position of the tile it is on
                canTurn = true;
            }
        }
        
        if(behaviour != BEHAVIOUR::PANIC_MODE){ // if the monster is not in PANIC_MODE
            target = nextTarget;
            nextTarget = computeNextTarget();
        }

        if(!hasAlreadyUpdatedDirection)
            move(elapsed); // move the monster
    }
    
    rectangleShape.setFillColor(rectangleColor);
}


void Monster::drawOnWindow(sf::RenderWindow& window) const{
    window.draw(rectangleShape);
    Character::drawOnWindow(window);
}
