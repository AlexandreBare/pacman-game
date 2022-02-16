#include "Tile.hpp"

#include <stdexcept>

using namespace sf;
using namespace std;

//
// Private Non-Member function
//

int setUniqueId(){
    static int newId = 0;
    return newId++;
}

//
// Public functions
//

Tile::~Tile(){}

Tile::Tile(TILE_TYPE _type, Coordinates _position){
    type = _type;
    position = _position;
    id = setUniqueId();
}

Tile::Tile(TILE_TYPE _type, Coordinates _position, Food _food){
    if(_type == TILE_TYPE::WALL)
        throw invalid_argument("A tile can not have a wall and a piece of food");
    type = _type;
    position = _position;
    food = _food;
    id = setUniqueId();
}

const Coordinates& Tile::getPosition() const{
    return position;
}

TILE_TYPE Tile::getType() const{
    return type;
}

bool Tile::hasWall() const{
    return type == TILE_TYPE::WALL;
}

bool Tile::hasFood() const{
    return !food.isEaten();
}

int Tile::eatFood(){
    if(!hasFood())
        throw domain_error("No food to eat on tile");
    
    return food.eat();
}

void Tile::update(sf::Time elapsed){
    food.update(elapsed);
}

bool Tile::hasContaminatedFood() const{
    if(!hasFood())
        return false;
    
    return food.isContaminated();
}

void Tile::contaminateFood(){
    food.contaminate();
}

const Food& Tile::getFood() const{
    return food;
}

void Tile::drawOnWindow(sf::RenderWindow& window) const{
    if(!food.isEaten())
        food.drawOnWindow(window);
}

bool Tile::operator==(const Tile& tile) const{
    return id == tile.id;
}
