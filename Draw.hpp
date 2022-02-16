#ifndef DEF_DRAW
#define DEF_DRAW

#include <SFML/Graphics.hpp>

/*
 * Abstract class responsible for all drawable objects in the game
 */
class Draw{
public :
    /*
     * Draw the drawable objects onscreen
     *
     * Argument:
     * window, the window to draw to
     */
    virtual void drawOnWindow(sf::RenderWindow& window) const = 0;
};

#endif
