#ifndef DEF_FOOD
#define DEF_FOOD

#include "Draw.hpp"
#include "Coordinates.hpp"

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

/*
 * Class responsible for each piece of food in the game
 */
class Food : Draw{
protected :
    int value;
    bool isEaten_ = true;
    bool isPowerPill_ = false;
    sf::Time contaminationTimer;
    sf::CircleShape circleShape;
    Coordinates position;
    
public :
    
    /*
     * Default Constructor
     */
    Food(){}
    
    /*
     * Constructor
     *
     * Arguments:
     * _position, the position of the food piece
     * radius, the radius of the food circle shape
     * _value, the score value of the food
     * _isPowerPill, whether the food is a power pill
     */
    Food(Coordinates _position, float radius, int _value, bool _isPowerPill = false);
    
    /*
     * Eat a piece of food
     *
     * Return:
     * the score associated to eating a piece of food
     */
    int eat();
    
    /*
     * Return:
     * whether the food is eaten yet
     */
    bool isEaten() const;
	
    /*
     * Return:
     * whether the food is a power pill
     */
    bool isPowerPill() const;
    
    
    /*
     * Update the food
     */
    void update(sf::Time elapsed);
        
    /*
     * Contaminate a piece of food
     */
    void contaminate();
    
    /*
     * Return:
     * whether the piece of food is contaminated
     */
    bool isContaminated() const;
    
    void drawOnWindow(sf::RenderWindow& window) const override;
};


#endif
