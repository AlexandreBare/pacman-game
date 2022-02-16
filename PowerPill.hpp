#ifndef DEF_POWERPILL
#define DEF_POWERPILL

#include "Food.hpp"

/*
 * Class responsible for each power pill, ie: big piece of food
 */
class PowerPill : public Food{
	public :
    
    /*
     * Constructor
     *
     * Argument:
     * position, the food position
     */
    PowerPill(Coordinates position);
};

#endif
