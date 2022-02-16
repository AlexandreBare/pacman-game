#ifndef DEF_COORDINATES
#define DEF_COORDINATES

#include <SFML/System.hpp>

/*
 * Class responsible for position, direction and vector objects in the 2D game.
 * The origin is at the top left corner, the x-axis is positive rightwards
 * and the y-axis is positive downwards
 */
class Coordinates : public sf::Vector2f{
public :
    
    /*
     * Default Constructor
     * Initialises a zero vector
     */
    Coordinates(){}
    
    /*
     * Constructor
     *
     * Arguments:
     * x, the x-axis coordinate
     * y, the y-axis coordinate
     */
    Coordinates(float x, float y) : sf::Vector2f(x, y){}
    
    /*
     * Constructor
     *
     * Arguments:
     * xy, the x-axis and y-axis coordinates
     */
    Coordinates(const float xy[]) : sf::Vector2f(xy[0], xy[1]){}
    
    /*
     * Overload operator*
     *
     * Argument:
     * lhs, the left-hand side object
     * factor, the right-hand side factor
     *
     * Return:
     * the coordinates object after mutliplication by the factor
     */
    friend Coordinates operator*(const Coordinates& lhs, float factor);
    
    /*
     * Overload operator+
     *
     * Argument:
     * lhs, the left-hand side object
     * rhs, the right-hand side object
     *
     * Return:
     * the Coordinates object after addition of the 2 Coordinates objects
     */
    friend Coordinates operator+(const Coordinates& lhs, const Coordinates& rhs);
    
    /*
     * Overload operator-
     *
     * Argument:
     * lhs, the left-hand side object
     * rhs, the right-hand side object
     *
     * Return:
     * the Coordinates object after the soustraction of the 2 Coordinates objects
     */
    friend Coordinates operator-(const Coordinates& lhs, const Coordinates& rhs);
    
    /*
     * Overload operator==
     *
     * Argument:
     * lhs, the left-hand side object
     * rhs, the right-hand side object
     *
     * Return:
     * whether lhs and rhs are similar
     */
    friend bool operator==(const Coordinates& lhs, const Coordinates& rhs);
    
    /*
     * Overload operator!=
     *
     * Argument:
     * lhs, the left-hand side object
     * rhs, the right-hand side object
     *
     * Return:
     * whether lhs and rhs are similar
     */
    friend bool operator!=(const Coordinates& lhs, const Coordinates& rhs);
    
    /*
     * Compute the distance between two 2D vectors
     *
     * Argument:
     * destination, the destination 2D vector
     *
     * Return:
     * the distance between the calling object and the given destination
     */
    float getDistanceToCoordinates(const Coordinates& destination) const;
    
    /*
     * Normalize the 2D vector
     */
     Coordinates normalize() const;
    
    /*
     * The coordinates can be constraint in a specific range for the game.
     * If out of range, coordinates are set back in the right range.
     *
     * Return:
     * the coordinates clamped in the right range
     */
    Coordinates clampCoordinates() const;
    
    /*
     * Argument:
     * position, the position
     *
     * Return:
     * whether the position is out of screen
     */
    static bool isOutOfScreen(const Coordinates& position);
    
    /*
     * Argument:
     * source, the source position
     * destination, the destination position
     *
     * Return:
     * the direction to go from source to destination
     */
    static Coordinates getDirectionBtwPositions(const Coordinates& source, const Coordinates& destination);
};

#endif
