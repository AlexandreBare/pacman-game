#include "Food.hpp"
#include "constants.hpp"

#include <stdexcept>

using namespace sf;
using namespace std;

static const sf::Color FOOD_COLOR(247, 192, 158, 255);
static const sf::Color FOOD_CONTAMINATED_COLOR = sf::Color::Magenta;
static const sf::Time FOOD_CONTAMINATION_TIME = sf::seconds(6.f);

//
// Public functions
//

Food::Food(Coordinates _position, float radius, int _value, bool _isPowerPill) : circleShape(radius){
    position = _position;
    value = _value;
    isEaten_ = false;
    isPowerPill_ = _isPowerPill;
    circleShape.setOrigin(radius, radius);
    circleShape.setPosition(position);
    circleShape.setFillColor(FOOD_COLOR);
}

int Food::eat(){
    if(isEaten_)
        throw domain_error("Food already eaten");
    
    isEaten_ = true;
    return value;
}

bool Food::isEaten() const{
    return isEaten_;
}

bool Food::isPowerPill() const{
    return isPowerPill_;
}

void Food::update(Time elapsed){
    if(contaminationTimer <= Time::Zero)
        circleShape.setFillColor(FOOD_COLOR);
    else
        contaminationTimer -= elapsed;
}

void Food::contaminate(){
    contaminationTimer = FOOD_CONTAMINATION_TIME;
    if(DEBUG == true)
        circleShape.setFillColor(FOOD_CONTAMINATED_COLOR);
}

bool Food::isContaminated() const{
    return contaminationTimer > Time::Zero;
}

void Food::drawOnWindow(sf::RenderWindow& window) const{
    if(isEaten_ == false){
        window.draw(circleShape);
    }
}
