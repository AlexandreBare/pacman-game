#ifndef DEF_MONSTER
#define DEF_MONSTER

#include "Character.hpp"
#include "Pacman.hpp" 
#include "Tile.hpp"
#include "Coordinates.hpp"
#include "constants.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>

/*
 * Abstract class responsible for square monsters
 */
class Monster : public Character{
protected :
    
    Tile target;
    Tile homeTarget;
    Tile nextTarget;
    
    Coordinates rootingDirection;
    Coordinates previousDirection;
    
    BEHAVIOUR behaviour;
    
    const Pacman* pacman;
    
    sf::RectangleShape rectangleShape;
    sf::Color monsterColor;
    sf::Color rectangleColor;
    
    int value;
    
    bool isInFrontOfMonsterHouse = false;
    bool isMovingToHouseCenter = false;
    bool isMonsterHouseMove = false;
    bool isDirectionFixedOnSlowTile = false;
    bool canTurn = false;
    bool canTurn180 = false; // can't do a U-turn, i.e. a turn of 180 degree
    
    /*
     * Move the monster
     *
     * Argument:
     * elapsed, the time elapsed since last update
     */
    void move(sf::Time elapsed);
    
    /*
     * Compute the next target tile the monster will move to
     * It computes one random adjacent tile to the monster's tile as target
     *
     * Return:
     * the next target tile
     */
    virtual const Tile& computeNextTarget() = 0;
    
    /*
     * Compute the direction of movement to go to the target tile
     *
     * Return:
     * the rooting direction to the target tile
     */
    Coordinates computeRootingDirection();
    
    /*
     * Compute specific actions in panic mode
     *
     * Argument:
     * panicTimer, the time left in panic mode
     */
    void flee(sf::Time panicTimer);
    
    /*
     * Revive the monster with a new immune system
     */
    void regenerate();

    /*
     * Constructor
     *
     * Argument:
     * _position, the starting position of the monster
     * _homeTarget, the home tile that the monster will target in scatter mode
     * _grid, a reference to the gameboard grid
     * _pacman, a reference to pacman
     * _monsterColor, the monster default color
     */
    Monster(Coordinates& _position, const Tile& _homeTarget, const Grid& _grid, const Pacman* _pacman, const sf::Color& _monsterColor);
    
public :
    
    /*
     * Kill the monster
     *
     * Return:
     * the score asssociated to a monster kill
     */
    int kill();
    
    /*
     * Update the monster at each frame
     *
     * Argument:
     * timeElapsed, the time elapsed since the last update
     * behaviourTimer, the time left in the current behaviour
     * _behaviour, the current behaviour
     */
    void update(sf::Time elapsed, sf::Time behaviourTimer, BEHAVIOUR _behaviour);
    
    void drawOnWindow(sf::RenderWindow& window) const override final;
};


#endif
