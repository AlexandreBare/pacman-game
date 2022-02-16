#ifndef DEF_GAMEPLAY
#define DEF_GAMEPLAY

#include "Grid.hpp"

#include "Statistics.hpp"

#include "Pacman.hpp"
#include "Monster.hpp"
#include "Shadow.hpp"
#include "Speedy.hpp"
#include "Bashful.hpp"
#include "Pokey.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>
#include <vector>

enum class OUTCOME{UNFINISHED, WIN, LOSE};

/*
 * Main class responsible for the managament of the game
 * and the user inputs reading
 */
class Gameplay : public Draw{
private :
    bool hasGameStarted = false;

    OUTCOME finalOutcome = OUTCOME::UNFINISHED;
    sf::Text message;
    sf::Font font;
    Statistics statistics;
    
    Grid grid;
    
    Pacman pacman;
    Shadow blinky;
    Speedy pinky;
    Bashful inky;
    Pokey clyde;
    std::vector<Monster*> monsters;
    
    BEHAVIOUR behaviour; // The monsters' behaviour
    BEHAVIOUR scatterChaseBehaviour; // A cycle of CHASE_MODE - SCATTER_MODE behaviour
    
    sf::Clock mainClock;
    sf::Time lastUpdate;
    sf::Time scatterChaseTimer;
    sf::Time panicModeTimer;
    sf::Time behaviourTimer;
    sf::Time infectionStartTime;
    
    /*
     * Compute the monsters' behaviour state
     */
    void computeBehaviour(sf::Time elapsed);
    
		
public :
    
    /*
     * Default constructor
     */
    Gameplay();
    
    /*
     * Update the game
     */
    void update();
    
    /*
     * Draw the game
     *
     * Argument:
     * window, the window to draw to
     */
    void drawOnWindow(sf::RenderWindow& window) const override;
};


#endif
