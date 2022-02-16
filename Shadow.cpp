#include "Shadow.hpp"

#include <stdexcept>

using namespace sf;
using namespace std;

static const Color SHADOW_COLOR = sf::Color::Red;

Shadow::Shadow(Coordinates _position, const Tile& _homeTarget, const Grid& _grid, const Pacman* _pacman) : Monster(_position, _homeTarget, _grid, _pacman, SHADOW_COLOR){
    direction = COORDINATES_DIRECTION.at(DIRECTION::LEFT);
}

const Tile& Shadow::computeNextTarget(){
    switch(behaviour){
        case BEHAVIOUR::CHASE_MODE:
            return pacman->getTile();
            break;
        case BEHAVIOUR::SCATTER_MODE:
            return homeTarget;
            break;
        default:
            throw domain_error("Invalid Behaviour Mode: fail to compute the next monster target tile");
            break;
    }
    
}
