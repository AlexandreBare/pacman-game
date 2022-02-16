#include "Gameplay.hpp"
#include "constants.hpp"
#include "Grid.hpp"
#include "Tile.hpp"
#include "Coordinates.hpp"

#include <SFML/Window.hpp>
#include <vector>
#include <cstdlib>
#include <ctime>

using namespace sf;
using namespace std;

// Game Messages
static const std::string READY_MESSAGE = "Ready?!";
static const std::string WIN_MESSAGE = "You win!";
static const std::string LOSE_MESSAGE = "Too bad!";

// Number of Monsters
static const int NB_MONSTERS = 4;

// Character Start Position
static const float PACMAN_START[] = {14.f * CELL_SIZE, 26.5f * CELL_SIZE};

static const float BLINKY_START[] = {14.f * CELL_SIZE, 14.5f * CELL_SIZE};
static const float PINKY_START[] = {14.f * CELL_SIZE, 17.5f * CELL_SIZE};
static const float INKY_START[] = {12.f * CELL_SIZE, 17.5f * CELL_SIZE};
static const float CLYDE_START[] = {16.f * CELL_SIZE, 17.5f * CELL_SIZE};

// Monster Target Tile Index
static const int BLINKY_HOME_TARGET[] = {25, 0};
static const int PINKY_HOME_TARGET[] = {2, 0};
static const int INKY_HOME_TARGET[] = {27, 35};
static const int CLYDE_HOME_TARGET[] = {0, 35};

// Time
static const Time SCATTER_MODE_DURATION = seconds(9.f);
static const Time CHASE_MODE_DURATION = seconds(30.f);
static const Time PANIC_MODE_DURATION = seconds(7.f);

static const Time INKY_START_TIME = seconds(15.f);
static const Time CLYDE_START_TIME = seconds(60.f);

// Number Food Eaten
static const int INKY_START_FOOD = 30;
static const int CLYDE_START_FOOD = 82;



//
// Public functions
//

Gameplay::Gameplay() : pacman(Coordinates(PACMAN_START), grid),
                       blinky(Coordinates(BLINKY_START),
                              grid(BLINKY_HOME_TARGET), grid, &pacman),
                       pinky(Coordinates(PINKY_START),
                              grid(PINKY_HOME_TARGET), grid, &pacman),
                       inky(Coordinates(INKY_START), grid(INKY_HOME_TARGET), grid, &pacman),
                       clyde(Coordinates(CLYDE_START), grid(CLYDE_HOME_TARGET), grid, &pacman)
{
                           
    if (!font.loadFromFile("press-start-2p-v8-latin-regular.ttf")) {
        throw invalid_argument("Could Not Load Font File");
    }
    message = Text(READY_MESSAGE, font, FONT_SIZE);
    message.setFillColor(TEXT_COLOR);
    sf::FloatRect textRect = message.getLocalBounds();
    message.setOrigin(textRect.width / 2.0f, textRect.height / 2.0f);
    message.setPosition(14 * CELL_SIZE, 20.5 * CELL_SIZE);
                           
    monsters.reserve(NB_MONSTERS);
    monsters.push_back(&blinky);
    monsters.push_back(&pinky);
    monsters.push_back(&inky);
    monsters.push_back(&clyde);
                           
    scatterChaseBehaviour = BEHAVIOUR::CHASE_MODE;
    scatterChaseTimer = SCATTER_MODE_DURATION + CHASE_MODE_DURATION;
    
    if(PLAY_WITH_INFECTION){
        srand(time(NULL));
        infectionStartTime = Time(milliseconds((rand() % LATEST_VIRUS.asMilliseconds())));
    }
}

void Gameplay::update(){
    if(finalOutcome != OUTCOME::UNFINISHED)
        return;
    
    DIRECTION direction = DIRECTION::ZERO;
    
    // Arrow Key Pressed ?
    if (Keyboard::isKeyPressed(Keyboard::Left)){
        direction = DIRECTION::LEFT;
    }else if (Keyboard::isKeyPressed(Keyboard::Right)){
        direction = DIRECTION::RIGHT;
    }else if (Keyboard::isKeyPressed(Keyboard::Down)){
        direction = DIRECTION::DOWN;
    }else if (Keyboard::isKeyPressed(Keyboard::Up)){
        direction = DIRECTION::UP;
    }
    
    if(hasGameStarted == false){ // if game has not yet started
        if(direction != DIRECTION::ZERO){ // if arrow key pressed
            hasGameStarted = true;
            message.setString("");
            mainClock.restart();
        }
    }else{
        // Time Management
        Time now = mainClock.getElapsedTime();
        Time elapsed;
        if(lastUpdate != Time::Zero){ // if it is not the first update
            elapsed = now - lastUpdate;
        }
        lastUpdate = now;
        
        // Add monsters to game
        int nbMonstersInPlay = 2;
        if(now >= CLYDE_START_TIME || statistics.getNbFoodEaten() >= CLYDE_START_FOOD)
            nbMonstersInPlay = 4;
        else if(now >= INKY_START_TIME || statistics.getNbFoodEaten() >= INKY_START_FOOD)
           nbMonstersInPlay = 3;
        
        if(PLAY_WITH_INFECTION){
            // Start infection
            if(infectionStartTime <= now && infectionStartTime >= Time::Zero){
                monsters[rand() % nbMonstersInPlay]->infect();
                infectionStartTime = Time::Zero;
            }
            if(infectionStartTime >= Time::Zero)
                infectionStartTime -= elapsed;
        }
        
        // Update Behaviour
        computeBehaviour(elapsed);
        
        if(panicModeTimer == Time::Zero)
            statistics.resetPowerPillMult(); // reset the doubling of the score that happens each time a monster is eaten
        
        Tile pacmanTile = pacman.getTile();
        if(grid.hasFoodOnTile(pacmanTile)){
            
            // Update Food & Score + Virus propagation on pacman
            if(grid.hasContaminatedFoodOnTile(pacmanTile))
                pacman.infect();
            statistics.update(grid.eatFoodOnTile(pacmanTile), true);
            if(statistics.getNbFoodEaten() == NB_TREATS + NB_POWER_PILLS){
                message.setString(WIN_MESSAGE);
                sf::FloatRect textRect = message.getLocalBounds();
                message.setOrigin(textRect.width / 2.f, textRect.height / 2.f);
                finalOutcome = OUTCOME::WIN;
                return;
            }
        }else
            
            // Update Pacman
            pacman.update(elapsed, direction, panicModeTimer);
         
        
        // Update monsters
        for(int i = 0; i < nbMonstersInPlay; i++){
            monsters[i]->update(elapsed, behaviourTimer, behaviour);
        }
        
        // Update the gameboard grid
        grid.update(elapsed);
        
        // Virus propagation on monsters
        for(auto* monster : monsters){
            if(!monster->isDead()){
                Tile monsterTile = monster->getTile();
                if(grid.hasFoodOnTile(monsterTile)){
                    if(monster->getInfectionState() != INFECTION_STATE::SAFE)
                        grid.contaminateFoodOnTile(monsterTile);
                    else if(grid.hasContaminatedFoodOnTile(monsterTile))
                        monster->infect();
                }
            }
        }
        
        // Collision detection & Score update + Virus propagation on pacman
        for(auto* monster : monsters){
            if(pacman.getTile() == monster->getTile() && !monster->isDead()){
                if(pacman.isInvincible()){
                    if(monster->getInfectionState() != INFECTION_STATE::SAFE)
                        pacman.infect();
                    
                    statistics.update(monster->kill(), false);
                }else{
                    message.setString(LOSE_MESSAGE);
                    sf::FloatRect textRect = message.getLocalBounds();
                    message.setOrigin(textRect.width / 2.f, textRect.height / 2.f);
                    finalOutcome = OUTCOME::LOSE;
                    return;
                }
            }
        }
    }
}

void Gameplay::drawOnWindow(RenderWindow& window) const{
    grid.drawOnWindow(window);
    if(finalOutcome != OUTCOME::LOSE)
        pacman.drawOnWindow(window);
   
    for(auto& monster : monsters){
        monster->drawOnWindow(window);
    }
    
    window.draw(message);
    statistics.drawOnWindow(window);
}



//
// Private functions
//

void Gameplay::computeBehaviour(Time elapsed){
    scatterChaseTimer -= elapsed;
    if(scatterChaseTimer <= Time::Zero){
        switch(scatterChaseBehaviour){
            case BEHAVIOUR::SCATTER_MODE:
                    scatterChaseBehaviour = BEHAVIOUR::CHASE_MODE;
                    scatterChaseTimer = CHASE_MODE_DURATION;
                break;
            case BEHAVIOUR::CHASE_MODE:
                    scatterChaseBehaviour = BEHAVIOUR::SCATTER_MODE;
                    scatterChaseTimer = SCATTER_MODE_DURATION;
                break;
            default: throw domain_error("Exception when computing the SCATTER-CHASE cycle: behaviour value undefined");
                break;
        }
    }
    
    if(panicModeTimer > Time::Zero){
        panicModeTimer -= elapsed;
        behaviourTimer = panicModeTimer;
    }else{
        panicModeTimer = Time::Zero;
        behaviour = scatterChaseBehaviour;
        behaviourTimer = scatterChaseTimer;
    }
    
    if(grid.hasFoodOnTile(pacman.getTile()) && pacman.getTile().getFood().isPowerPill()){
        behaviour = BEHAVIOUR::PANIC_MODE;
        panicModeTimer += PANIC_MODE_DURATION;
        behaviourTimer = panicModeTimer;
    }
}
