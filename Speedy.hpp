#ifndef DEF_SPEEDY
#define DEF_SPEEDY

#include "Monster.hpp"
#include "Grid.hpp"
#include "Tile.hpp"
#include "Coordinates.hpp"
#include "Pacman.hpp"

/*
 * Class responsible for the monsters of type Speedy
 */
class Speedy : public Monster{
public :
    
    /*
     * Constructor
     *
     * Argument:
     * _position, the starting position of the speedy monster
     * _homeTarget, the home tile the speedy monster will target in scatter mode
     * _grid, a reference to the gameboard grid
     * _pacman, a pointer to pacman
     */
    Speedy(Coordinates _position, const Tile& _homeTarget, const Grid& _grid, const Pacman* _pacman);
    
    
protected :
    
    /*
     * Compute the next target tile the speedy monster will move to
     *
     * Return:
     * the next target tile
     */
    const Tile& computeNextTarget() override;
};

#endif
