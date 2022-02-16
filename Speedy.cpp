#include "Speedy.hpp"

#include <stdexcept>

using namespace sf;
using namespace std;

static const Color SPEEDY_COLOR(250, 197, 246, 255);
static const int SPEEDY_LOOK_AHEAD = 4;

Speedy::Speedy(Coordinates _position, const Tile& _homeTarget, const Grid& _grid, const Pacman* _pacman) : Monster(_position, _homeTarget, _grid, _pacman, SPEEDY_COLOR){
    isMonsterHouseMove = true;
}

const Tile& Speedy::computeNextTarget(){
    switch(behaviour){
        case BEHAVIOUR::CHASE_MODE:{
            int tileIndexes[2];
            Grid::getTileIndexesFromPosition(pacman->getPosition(), tileIndexes);
            Coordinates pacmanDir = pacman->getDirection();
            tileIndexes[0] += SPEEDY_LOOK_AHEAD * pacmanDir.x;
            tileIndexes[1] += SPEEDY_LOOK_AHEAD * pacmanDir.y;
            return grid(tileIndexes);
            break;
        }
        case BEHAVIOUR::SCATTER_MODE:
            return homeTarget;
            break;
        default:
            throw domain_error("Invalid Behaviour Mode: fail to compute the next monster target tile");
            break;
    }
    
}
