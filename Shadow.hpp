#ifndef DEF_SHADOW
#define DEF_SHADOW

#include "Monster.hpp"
#include "Grid.hpp"
#include "Tile.hpp"
#include "Coordinates.hpp"
#include "Pacman.hpp"

/*
 * Class responsible for the monsters of type Shadow
 */
class Shadow : public Monster{
public :
    
    /*
     * Constructor
     *
     * Argument:
     * _position, the starting position of the shadow monster
     * _homeTarget, the home tile the shadow monster will target in scatter mode
     * _grid, a reference to the gameboard grid
     * _pacman, a pointer to pacman
     */
    Shadow(Coordinates _position, const Tile& _homeTarget, const Grid& _grid, const Pacman* _pacman);
	
    
protected :
    
    /*
     * Compute the next target tile the shadow monster will move to
     *
     * Return:
     * the next target tile
     */
    const Tile& computeNextTarget() override;
};


#endif
