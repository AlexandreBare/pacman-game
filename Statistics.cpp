#include "Statistics.hpp"
#include "constants.hpp"

using namespace std;

static const int SCORE_LENGTH = 4;


//
// Public functions
//


Statistics::Statistics(){
    font.loadFromFile("press-start-2p-v8-latin-regular.ttf");
    scoreText.setFont(font);
    scoreText.setString(string(SCORE_LENGTH, '0'));
    scoreText.setCharacterSize(FONT_SIZE);
    scoreText.setFillColor(TEXT_COLOR);
    sf::FloatRect textRect = scoreText.getLocalBounds();
    scoreText.setOrigin(textRect.width / 2.f, textRect.height / 2.f);
    scoreText.setPosition(14 * CELL_SIZE, 1.5f * CELL_SIZE);
}

void Statistics::update(int toAdd, bool isFood){
    if(isFood)
        nbFoodEaten++;
    else{
        toAdd *= powerPillMult;
        powerPillMult *= 2;
    }
    score += toAdd;
    string scoreString = to_string(score);
    string zeros(SCORE_LENGTH - scoreString.size(), '0');
    scoreString = zeros + scoreString;
    scoreText.setString(scoreString);
}

void Statistics::resetPowerPillMult(){
    powerPillMult = 1;
}

int Statistics::getNbFoodEaten() const{
    return nbFoodEaten;
}

void Statistics::drawOnWindow(sf::RenderWindow& window) const{
    window.draw(scoreText);
}

