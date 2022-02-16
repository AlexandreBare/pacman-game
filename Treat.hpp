#ifndef DEF_TREAT
#define DEF_TREAT

#include "Food.hpp"
#include "Coordinates.hpp"


/*
 * Class responsible for each treat, ie: small piece of food
 */
class Treat : public Food{
public :
    /*
     * Constructor
     *
     * Argument:
     * position, the food position
     */
    Treat(Coordinates position);
};


#endif
