#ifndef DEF_GRID
#define DEF_GRID

#include "Draw.hpp"
#include "Tile.hpp"
#include "Coordinates.hpp"

#include <SFML/Graphics.hpp>
#include <vector>

static const int NB_TREATS = 240;
static const int NB_POWER_PILLS = 4;

/*
 * Class responsible for the gameboard grid of tiles
 */
class Grid : public Draw{
private :
    std::vector<std::vector<Tile>> onScreenTiles; // The gameboard tiles
    std::vector<Tile> offScreenTiles; // A temporary buffer for the immediatly needed off-screen tiles
    int head = 0; // the index of the cyclic vector offScreenTiles
    
    static void drawLines(sf::RenderWindow& window);
    static void drawWall(float length, float large, float i, float y, sf::RenderWindow& window);
    static void drawWalls(sf::RenderWindow& window);
    
    /*
     * Arguments:
     * row, row index
     * col, col index
     *
     * Return:
     * whether the tile given by its indexes is off the screen or not
     */
    bool isOffScreenTile(int row, int col) const;
    
    /*
     * Arguments:
     * row, row index
     * col, col index
     *
     * Return:
     * the tile (either on-screen or off-screen) at the indexes given
     */
    const Tile& getTile(int row, int col);
    
    /*
     * Arguments:
     * row, row index
     * col, col index
     *
     * Return:
     * the on-screen tile at the indexes given
     */
    const Tile& getOnScreenTile(int row, int col) const;
    
public :
    
    /*
     * Default constructor
     */
    Grid();
    
    /*
     * Argument:
     * position, the coordinates object of a position
     *
     * Return:
     * a reference to the tile corresponding to the given position;
     * In case of a position out of range, a reference to a newly created tile is sent
     */
    const Tile& getTileFromPosition(const Coordinates& position);
    
    /*
     * This operator is used to access a tile of the grid
     * based on its indexes (0 is the first index)
     * Example: grid(3, 4) returns the tile at the fourth row and fifth column
     *
     * Arguments:
     * row, the row index of the grid
     * col, the column index of the grid
     *
     * Return:
     * a reference to the tile asked;
     * In case of a negative index or an index out of the upper bound,
     * a reference to a newly created tile is sent
     */
    const Tile& operator()(int row, int col);
    
    /*
     * This operator is used to access a tile of the grid
     * based on its indexes
     * Example: grid(int[]{3, 4}) returns the tile at the fourth row and fifth column
     *
     * Arguments:
     * rowcol, the row and column indexes of the grid
     *
     * Return:
     * a reference to the tile asked;
     * In case of a negative index or an index out of the upper bound,
     * a reference to a newly created tile is sent
     */
    const Tile& operator()(const int rowcol[]);
    
    /*
     * Update the grid
     *
     * Argument:
     * elapsed, the time since the last update
     */
    void update(sf::Time elapsed);
    
    /*
     * Argument:
     * tile, a tile
     *
     * Return:
     * whether there is a piece of food on the given tile
     */
    bool hasFoodOnTile(const Tile& tile) const;
    
    /*
     * Eat the piece of food on a given tile
     *
     * Argument:
     * tile, the tile
     *
     * Return:
     * the value of the food eaten
     */
    int eatFoodOnTile(const Tile& tile);
    
   /*
    * Argument:
    * tile, a tile
    *
    * Return:
    * whether the piece of food on a given tile is contaminated
    */
    bool hasContaminatedFoodOnTile(const Tile& tile) const;
    
    /*
     * Contaminate the piece of food on a given tile
     *
     * Argument:
     * tile, the tile
     */
    void contaminateFoodOnTile(const Tile& tile);
    
    /*
     * Display the grid
     *
     * Argument:
     * window, the window on which to draw
     */
    void drawOnWindow(sf::RenderWindow& window) const override;
    
    /*
     * Convert a position in tile indexes
     *
     * Arguments:
     * res, the result of the function : the tile indexes corresponding to the given position
     * position, the position
     */
    static void getTileIndexesFromPosition(const Coordinates& position, int res[]);
    
    /*
     * Compute the number of tiles between two tiles
     *
     * Argument:
     * destination, the destination tile
     *
     * Return:
     * the number of tiles between the callg object and the destination tile
     */
    static int getTileDistance(const Tile& source, const Tile& destination);
};


#endif
