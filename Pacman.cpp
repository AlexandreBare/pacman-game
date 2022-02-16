#include "Pacman.hpp"
#include "constants.hpp"

#include <cfloat>
#include <cmath>

using namespace sf;
using namespace std;

static const float PACMAN_RADIUS = 0.8f * REF_SIZE;
static const float PACMAN_SPEED = 1.f * REF_SPEED;
static const float PACMAN_POWERPILL_SPEED = 1.05f * REF_SPEED;
static const sf::Color PACMAN_COLOR = sf::Color::Yellow;


//
// Public Functions
//

Pacman::Pacman(const Coordinates& _position, const Grid& _grid) : circleShape(PACMAN_RADIUS){
    position = _position;
    grid = _grid;
    tile = grid.getTileFromPosition(position);
    speed = PACMAN_SPEED;
    infectionState = INFECTION_STATE::SAFE;
    circleShape.setFillColor(PACMAN_COLOR);
    circleShape.setOrigin(PACMAN_RADIUS, PACMAN_RADIUS);
    circleShape.setPosition(position);
}

bool Pacman::isInvincible() const{
    return isInvincible_;
}

void Pacman::update(sf::Time elapsed, DIRECTION turnDirection, sf::Time panicTimer){
    updateVirus(elapsed);
    if(panicTimer == Time::Zero){
        isInvincible_ = false;
        speed = PACMAN_SPEED;
    }else{
        isInvincible_ = true;
        speed = PACMAN_POWERPILL_SPEED;
    }
    
    move(COORDINATES_DIRECTION.at(turnDirection), elapsed);
}

void Pacman::drawOnWindow(sf::RenderWindow& window) const{
    window.draw(circleShape);
    Character::drawOnWindow(window);
}


//
// Private Functions
//

void Pacman::move(const Coordinates& turnDirection, sf::Time elapsed){
    if(!Coordinates::isOutOfScreen(position)){ // if pacman is not out of the gameboard
        if(isCuttingCorner == true){ // if pacman is currently cutting a corner (its direction is thus fixed until it's done)
            Coordinates directionToTarget = Coordinates::getDirectionBtwPositions(position, target); // direction between pacman and its target tile
            if(directionToTarget != direction){ // if pacman has passed the target tile
                direction = nextDirection;
                position = target;
                isCuttingCorner = false;
                canUserMakePacmanTurn = true;
            }
        }else{
            // Compute the next tile in the current direction
            int tileIndexes[2];
            Grid::getTileIndexesFromPosition(position, tileIndexes);
            Coordinates vectorDir = direction;
            Tile nextTile = grid(tileIndexes[0] + vectorDir.x, tileIndexes[1] + vectorDir.y);
            
            Coordinates directionToTile = Coordinates::getDirectionBtwPositions(position, tile.getPosition()); // Get direction between pacman and the current tile
            if(directionToTile != direction && nextTile.hasWall()){ // if pacman has passed the middle position of the tile AND the next tile has a wall
                position = tile.getPosition();
                direction = COORDINATES_DIRECTION.at(DIRECTION::ZERO); // stop pacman by setting a ZERO direction
            }
            
            if(canUserMakePacmanTurn == true){ // if the user can still give an order for pacman to turn
                if(turnDirection != COORDINATES_DIRECTION.at(DIRECTION::ZERO) && turnDirection != direction){ // if the user wants to make pacman turn AND if the direction of turn is different from the current one
                    
                    // Compute the next tile in the direction of turn
                    int tileIndexes[2];
                    Grid::getTileIndexesFromPosition(position, tileIndexes);
                    Tile nextTile = grid(tileIndexes[0] + turnDirection.x, tileIndexes[1] + turnDirection.y);
                    if(!nextTile.hasWall()){ // if the next tile has no wall
                        if(canCutCorner(turnDirection)){ // if pacman can cut the corner, do so
                            Coordinates halfwayDir = (direction + turnDirection).normalize();
                            
                            if(abs(turnDirection.y) > FLT_EPSILON){
                                float y = (tile.getPosition().x - position.x) *  halfwayDir.y / halfwayDir.x + position.y;
                                target = Coordinates(tile.getPosition().x, y);
                            }else{ // if abs(turnDirection.y) > FLT_EPSILON
                                float x = (tile.getPosition().y - position.y) *  halfwayDir.x / halfwayDir.y + position.x;
                                target = Coordinates(x, tile.getPosition().y);
                            }
                            nextDirection = turnDirection;
                            direction = halfwayDir;
                            isCuttingCorner = true;
                            canUserMakePacmanTurn = false;
                        }else{
                            if(direction == COORDINATES_DIRECTION.at(DIRECTION::ZERO)){ // if pacman is not moving
                                direction = turnDirection;
                            }else if(directionToTile == direction){ // if pacman has not passed the middle position of the tile
                                nextDirection = turnDirection; // save pacman turn direction so that he can turn later without cutting the corner
                                canUserMakePacmanTurn = false;
                            }
                        }
                    }
                }
            
            // if the user can no more give a new order for pacman to turn because one is already waiting
            }else if(directionToTile != direction){ // if pacman has passed the middle position of the tile it is on
                position = tile.getPosition();
                direction = nextDirection;
                canUserMakePacmanTurn = true;
            }
        }
    }
    updatePosition(elapsed); // update pacman position
    circleShape.setPosition(position); // and its shape too
    
}

bool Pacman::canCutCorner(const Coordinates& turnDirection) const{
     if(canUserMakePacmanTurn == false || direction == turnDirection || direction == turnDirection * -1 || turnDirection == COORDINATES_DIRECTION.at(DIRECTION::ZERO))
         // if the user can no more give a new order for pacman to turn because one is already waiting
         // OR if pacman's direction won't change,
         // OR if the user want pacman to do a 'U'-turn
         // OR if pacman will stop moving
           return false;
    
    if(direction == COORDINATES_DIRECTION.at(DIRECTION::ZERO)) // if pacman is not currently moving
        return false;
    
    Coordinates directionToTile = Coordinates::getDirectionBtwPositions(position, tile.getPosition());
    if(directionToTile == direction){ // if pacman has not yet passed the middle position of the tile it is on
        return true;
    }
    
    return false;
}
