#ifndef DEF_TILE
#define DEF_TILE

#include "Draw.hpp"
#include "Food.hpp"
#include "Coordinates.hpp"

#include <SFML/Graphics.hpp>
#include <SFML/System.hpp>


enum class TILE_TYPE{NORMAL, ESCAPE, SLOW, WALL};

/*
 * Class responsible for each gameboard tile
 */
class Tile : public Draw{
private :
    TILE_TYPE type;
    Coordinates position;
    Food food;
    int id;
    
public :
    
    /*
     * Default constructor
     */
    Tile(){}
    
    /*
     * Virtual destructor
     */
    virtual ~Tile();
    
    /*
     * Constructor
     *
     * Argument:
     * _type, the type of tile
     * _position, the middle position of the tile
     */
    Tile(TILE_TYPE _type, Coordinates _position);
    
    /*
     * Constructor
     *
     * Argument:
     * _type, the type of tile
     * _position, the middle position of the tile
     * _food, the piece of food that is on the tile
     */
    Tile(TILE_TYPE _type, Coordinates _position, Food _food);
    
    /*
     * Return:
     * the position of the tile center
     */
    const Coordinates& getPosition() const;
    
    /*
     * Return:
     * the tile type
     */
    TILE_TYPE getType() const;
    
    /*
     * Return:
     * whether there is a wall on the tile
     */
    bool hasWall() const;
    
    
    /*
     * Return:
     * whether there is a piece of food on the tile
     */
    bool hasFood() const;
    
    /*
     * Eat the food on the tile
     *
     * Argument:
     * the score value associated to eating the piece of food
     *
     * Throw:
     * domain_error if the tile did not contain any food
     */
    int eatFood();
    
    /*
     * Update the tile
     *
     * Argument:
     * the time elapsed since last update
     */
    void update(sf::Time elapsed);
    
   /*
    * Return:
    * whether the food on the tile is contaminated
    */
    bool hasContaminatedFood() const;
    
    /*
     * Contaminate the food on the tile
     *
     * Throw:
     * domain_error if the tile did not contain any food
     */
    void contaminateFood();
    
    /*
     * Get the food on the tile
     *
     * Return:
     * the food on the tile
     */
    const Food& getFood() const;
    
    /*
     * Argument:
     * tile, the tile to compare to
     *
     * Return:
     * whether the calling tile and the given tile are similar
     */
    bool operator==(const Tile& tile) const;
    
    /*
     * Display the tile onscreen
     *
     * Argument:
     * window, the window to draw on
     */
    void drawOnWindow(sf::RenderWindow& window) const override;
};


#endif
