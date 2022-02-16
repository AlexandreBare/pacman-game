#include "Bashful.hpp"

#include <stdexcept>

using namespace sf;
using namespace std;

static const Color BASHFUL_COLOR = sf::Color::Cyan;
static const int BASHFUL_LOOK_AHEAD = 2;

Bashful::Bashful(Coordinates _position, const Tile& _homeTarget, const Grid& _grid, const Pacman* _pacman) : Monster(_position, _homeTarget, _grid, _pacman, BASHFUL_COLOR){
    isMonsterHouseMove = true;
    isMovingToHouseCenter = true;
}

const Tile& Bashful::computeNextTarget(){
    switch(behaviour){
        case BEHAVIOUR::CHASE_MODE:{
            int tileIndexes[2];
            Grid::getTileIndexesFromPosition(pacman->getPosition(), tileIndexes);
            Coordinates pacmanDir = pacman->getDirection();
            tileIndexes[0] += BASHFUL_LOOK_AHEAD * pacmanDir.x;
            tileIndexes[1] += BASHFUL_LOOK_AHEAD * pacmanDir.y;
            Tile inFrontOfPacmanTile = grid(tileIndexes);
            Coordinates targetVector = (inFrontOfPacmanTile.getPosition() - position) * 2.f;
            return grid.getTileFromPosition(position + targetVector);
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
