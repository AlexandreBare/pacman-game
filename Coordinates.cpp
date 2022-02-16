#include "Coordinates.hpp"

#include "constants.hpp"

#include <cmath>
#include <stdexcept>
#include <cfloat>

using namespace sf;
using namespace std;

static const float MIN_X_CLAMP = MIN_X - NB_OFF_SCREEN_TILES_MARGIN * CELL_SIZE;
static const float MAX_X_CLAMP = MAX_X + NB_OFF_SCREEN_TILES_MARGIN * CELL_SIZE;
static const float MIN_Y_CLAMP = MIN_Y - NB_OFF_SCREEN_TILES_MARGIN * CELL_SIZE;
static const float MAX_Y_CLAMP = MAX_Y + NB_OFF_SCREEN_TILES_MARGIN * CELL_SIZE;


//
// Public Functions
//

float Coordinates::getDistanceToCoordinates(const Coordinates& destination) const{
    return sqrt(pow(x - destination.x, 2)
                + pow(y - destination.y, 2));
}

Coordinates Coordinates::normalize() const{
    Coordinates res = *this;
    float length = sqrt(pow(res.x, 2) + pow(res.y, 2));
    if(abs(length) > FLT_EPSILON)
        res /= length;
    else
        throw domain_error("Null vector can't be normalized");
    return Coordinates(res.x, res.y);
}

Coordinates Coordinates::clampCoordinates() const{
    float newX = x, newY = y;
    
    if(x > MAX_X_CLAMP)
        newX = MIN_X_CLAMP;
    else if(x < MIN_X_CLAMP)
        newX = MAX_X_CLAMP;
    
    if(y > MAX_Y_CLAMP)
        newY = MIN_Y_CLAMP;
    else if(y < MIN_Y_CLAMP)
        newY = MAX_Y_CLAMP;
        
    return Coordinates(newX, newY);
}



//
// Public Static Functions
//

bool Coordinates::isOutOfScreen(const Coordinates& position){
    return position.x < MIN_X || position.y < MIN_Y || position.x > MAX_X || position.y > MAX_Y;
}

Coordinates Coordinates::getDirectionBtwPositions(const Coordinates& source, const Coordinates& destination){
    Coordinates diff = destination - source;
    if(abs(diff.x) > FLT_EPSILON)
        diff.x /= abs(diff.x);
    if(abs(diff.y) > FLT_EPSILON)
        diff.y /= abs(diff.y);
    float length = sqrt(pow(diff.x, 2) + pow(diff.y, 2));
    if(length > FLT_EPSILON)
        diff /= length;
    return Coordinates(diff.x, diff.y);
}



//
// Non-Member Functions
//

Coordinates operator*(const Coordinates& lhs, float factor){
    Coordinates res = lhs;
    res *= factor;
    return res;
}

Coordinates operator+(const Coordinates& lhs, const Coordinates& rhs){
    Coordinates res = lhs;
    res += rhs;
    return res;
}

Coordinates operator-(const Coordinates& lhs, const Coordinates& rhs){
    Coordinates res = lhs;
    res -= rhs;
    return res;
}

bool operator==(const Coordinates& lhs, const Coordinates& rhs){
    return abs(lhs.x - rhs.x) < FLT_EPSILON && abs(lhs.y - rhs.y) < FLT_EPSILON;
}

bool operator!=(const Coordinates& lhs, const Coordinates& rhs){
    return !(lhs == rhs);
}
