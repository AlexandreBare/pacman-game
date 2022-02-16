#ifndef DEF_CHARACTER
#define DEF_CHARACTER

#include "Draw.hpp"
#include "Grid.hpp"
#include "Tile.hpp"
#include "Coordinates.hpp"

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>
#include <map>

static const int NB_DIRECTIONS = 4;
static const sf::Color CHARACTER_SICK_COLOR(152, 251, 152); // pale green

enum class DIRECTION{ZERO = -1, LEFT = 0, DOWN, RIGHT, UP};
static const std::map<DIRECTION, Coordinates> COORDINATES_DIRECTION =
                                                            {{DIRECTION::ZERO, Coordinates()},
                                                            {DIRECTION::LEFT, Coordinates(-1, 0)},
                                                            {DIRECTION::DOWN, Coordinates(0, 1)},
                                                            {DIRECTION::RIGHT, Coordinates(1, 0)},
                                                            {DIRECTION::UP, Coordinates(0, -1)}};
enum class INFECTION_STATE{SAFE, INCUBATION, SICK};
enum class BEHAVIOUR{SCATTER_MODE, CHASE_MODE, PANIC_MODE};

/*
 * Class responsible for the game characters
 */
class Character : public Draw{
protected :
    
    Grid grid;
    Tile tile;
    
    sf::CircleShape sickCircle;
    sf::CircleShape incubationCircle;
    sf::Time infectionTimer;
    INFECTION_STATE infectionState = INFECTION_STATE::SAFE;
    bool isImmune_ = false;
    
    Coordinates position;
    Coordinates direction;

    float speed;
    
    bool isDead_ = false;
    
    /*
     * Update the character's position by a given translation
     *
     * Argument:
     * elapsed, the time since the last position update
     */
    void updatePosition(sf::Time elapsed);
    
    /*
     * Update the virus on the character
     *
     * Argument:
     * elapsed, the time since the last virus update
     */
    void updateVirus(sf::Time elapsed);
    
    /*
     * Default Constructor
     */
    Character();


private :
    
    /*
     * Update the tile the character is in based on its position
     */
    void updateTile();
		
public :
    
    /*
     * Return:
     * the tile on which the character is
     */
    const Tile& getTile() const;
    
    /*
     * Return:
     * the charachter's direction of movement
     */
    const Coordinates& getDirection() const;
    
    /*
     * Return:
     * the character's position
     */
    const Coordinates& getPosition() const;
    
    /*
     * Return:
     * whether the character is dead
     */
    bool isDead() const;
    
    /*
     * Return:
     * whether the character is immune, i.e. can't be reinfected
     */
    bool isImmune() const;
    
    /*
     * Return:
     * the character's infection state
     */
    INFECTION_STATE getInfectionState() const;
    
    /*
     * Infect the character, ie: the character enters the incubation state
     */
    void infect();
    
    void drawOnWindow(sf::RenderWindow& window) const override;
    
};


#endif
