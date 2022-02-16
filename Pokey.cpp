#include "Pokey.hpp"

#include <stdexcept>

using namespace sf;
using namespace std;

static const Color POKEY_COLOR(247, 187, 20, 255);
static const int POKEY_DETECTION_RANGE = 8;

Pokey::Pokey(Coordinates _position, const Tile& _homeTarget, const Grid& _grid, const Pacman* _pacman) : Monster(_position, _homeTarget, _grid, _pacman, POKEY_COLOR){
    isMonsterHouseMove = true;
    isMovingToHouseCenter = true;
}

const Tile& Pokey::computeNextTarget(){
    switch(behaviour){
        case BEHAVIOUR::CHASE_MODE:
            if(Grid::getTileDistance(getTile(), pacman->getTile()) > POKEY_DETECTION_RANGE)
                return pacman->getTile();
            return homeTarget;
            break;
        case BEHAVIOUR::SCATTER_MODE:
            return homeTarget;
            break;
        default:
            throw domain_error("Invalid Behaviour Mode: fail to compute the next monster target tile");
            break;
    }
    
}
