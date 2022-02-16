#ifndef DEF_POKEY
#define DEF_POKEY

#include "Monster.hpp"
#include "Grid.hpp"
#include "Tile.hpp"
#include "Coordinates.hpp"
#include "Pacman.hpp"

/*
 * Class responsible for the monsters of type Pokey
 */
class Pokey : public Monster{
public :
    
    /*
     * Constructor
     *
     * Argument:
     * _position, the starting position of the shadow monster
     * _homeTarget, the home tile the pokey monster will target in scatter mode
     * _grid, a reference to the gameboard grid
     * _pacman, a pointer to pacman
     */
    Pokey(Coordinates _position, const Tile& _homeTarget, const Grid& _grid, const Pacman* _pacman);
    
    
protected :
    
    /*
     * Compute the next target tile the pokey monster will move to
     *
     * Return:
     * the next target tile
     */
    const Tile& computeNextTarget() override;
};

#endif
