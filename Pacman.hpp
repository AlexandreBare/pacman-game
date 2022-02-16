#ifndef DEF_PACMAN
#define DEF_PACMAN

#include "Character.hpp"
#include "Statistics.hpp"
#include "Coordinates.hpp"

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <vector>

class Monster; // forward declaration

/*
 * Class responsible for the pacman characters
 */
class Pacman : public Character{
private :
    bool canUserMakePacmanTurn = true; // whether pacman is allowed to turn following the user input
    bool isCuttingCorner = false;
    bool isInvincible_ = false;
    
    Coordinates nextDirection;
    Coordinates target;
    sf::CircleShape circleShape;

public :
    
    /*
     * Default constructor
     */
    Pacman(){}
    
    /*
     * Constructor
     *
     * Argument:
     * _position, the starting position of pacman
     * _grid, the gameboard grid
     */
    Pacman(const Coordinates& position, const Grid& _grid);
    
    /*
     * Return:
     * Whether pacman is invincible
     */
    bool isInvincible() const;
    
    /*
     * Update pacman at each frame
     *
     * Argument:
     * timeElapsed, the time elapsed since the last update
     * turnDirection, the direction of movement
     * panicTimer, the time left in panic mode
     */
    void update(sf::Time elapsed, DIRECTION turnDirection, sf::Time panicTimer);
    
    void drawOnWindow(sf::RenderWindow& window) const override;
    
private :
    
    /*
     * Move pacman
     *
     * Argument:
     * turnDirection, the direction of movement
     * elapsed, the time since last move
     */
    void move(const Coordinates& turnDirection, sf::Time elapsed);
    
    
    /*
     * Compute whether pacman can make a turn and "cut a corner"
     *
     * Argument:
     * turnDirection, the direction of movement
     *
     * Return:
     * whether pacman can make a turn and "cut a corner"
     */
    bool canCutCorner(const Coordinates& turnDirection) const;
};


#endif
