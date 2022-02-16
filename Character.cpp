#include "Character.hpp"
#include "constants.hpp"

using namespace sf;
using namespace std;

static const float CHARACTER_SICK_SPEED = 0.2f * REF_SIZE;
static const float CHARACTER_SICK_CIRCLE_RADIUS = CELL_SIZE / 2;
static const float CHARACTER_INCUBATION_CIRCLE_RADIUS = REF_SIZE / 6.f;
static const sf::Color CHARACTER_INCUBATION_CIRCLE_COLOR = sf::Color::Magenta;



//
// Public functions
//

const Tile& Character::getTile() const{
    return tile;
}

const Coordinates& Character::getDirection() const{
    return direction;
}

const Coordinates& Character::getPosition() const{
    return position;
}

bool Character::isDead() const{
    return isDead_;
}

bool Character::isImmune() const{
    return isImmune_;
}

INFECTION_STATE Character::getInfectionState() const{
    return infectionState;
}

void Character::infect(){
    if(isImmune() || infectionState != INFECTION_STATE::SAFE)
        return;
    
    infectionState = INFECTION_STATE::INCUBATION;
    infectionTimer = INCUBATION_TIME + SICK_TIME;
}

void Character::drawOnWindow(sf::RenderWindow& window) const{
    if(infectionState == INFECTION_STATE::SICK)
        window.draw(sickCircle);
    if(DEBUG == true){
       if(infectionState == INFECTION_STATE::INCUBATION)
           window.draw(incubationCircle);
    }
}

//
// Protected functions
//

Character::Character() : sickCircle(CHARACTER_SICK_CIRCLE_RADIUS), incubationCircle(CHARACTER_INCUBATION_CIRCLE_RADIUS){
    sickCircle.setFillColor(CHARACTER_SICK_COLOR);
    sickCircle.setOrigin(CHARACTER_SICK_CIRCLE_RADIUS, CHARACTER_SICK_CIRCLE_RADIUS);
    incubationCircle.setFillColor(CHARACTER_INCUBATION_CIRCLE_COLOR);
    incubationCircle.setOrigin(CHARACTER_INCUBATION_CIRCLE_RADIUS, CHARACTER_INCUBATION_CIRCLE_RADIUS);
}

void Character::updatePosition(Time elapsed){
    if(infectionState == INFECTION_STATE::SICK)
        speed = CHARACTER_SICK_SPEED;
    position += direction * elapsed.asSeconds() * speed;
    position = position.clampCoordinates();
    updateTile();
    sickCircle.setPosition(position);
    incubationCircle.setPosition(position);
}

void Character::updateVirus(sf::Time elapsed){
    if(infectionState == INFECTION_STATE::INCUBATION){
        infectionTimer -= elapsed;
        if(infectionTimer <= SICK_TIME){
            infectionState = INFECTION_STATE::SICK;
            isImmune_ = true;
        }
    }else if(infectionState == INFECTION_STATE::SICK){
        infectionTimer -= elapsed;
        if(infectionTimer <= Time::Zero)
            infectionState = INFECTION_STATE::SAFE;
    }
}


//
// Private Functions
//

void Character::updateTile(){
    tile = grid.getTileFromPosition(position);
}


