#ifndef DEF_STATISTICS
#define DEF_STATISTICS

#include "Draw.hpp"

#include <SFML/Graphics.hpp>

/*
 * Class responsible for managing a few statistics on the game
 */
class Statistics : public Draw{
private :
    int score = 0;
    int nbFoodEaten = 0;
    int powerPillMult = 1;
    sf::Font font;
    sf::Text scoreText;
		
public :
    
    /*
     * Default Contructor
     */
    Statistics();
    
    /*
     * Update the score and derived statistics
     *
     * Arguments:
     * toAdd, a score value to add to the current score
     * isFood, whether the added score value comes from a new piece of food eaten
     */
    void update(int toAdd, bool isFood);
    
    /*
     * Reset the power pill score multiplier
     * relative to the number of power pill eaten in a row
     *
     * Arguments:
     * toAdd, a score value to add to the current score
     * isFood, whether the added score value comes from a new piece of food eaten
     */
    void resetPowerPillMult();
    
    /*
     * Return:
     * the number of food dot eaten since the beginning of the game
     */
    int getNbFoodEaten() const;
    
    void drawOnWindow(sf::RenderWindow& window) const override;
};


#endif
