#ifndef constants_hpp
#define constants_hpp

#include <SFML/System.hpp>
#include <SFML/Graphics.hpp>

// set to false to play the normal -Pacman- game,
// set to true to play with infection -PACoronam-
static const bool PLAY_WITH_INFECTION = false;


static const int FPS = 60;

static const bool DEBUG = false;

static const float CELL_SIZE = 19.f;
static const int GRID_SIZE[] = {28, 36};

static const float MIN_X = 0.f;
static const float MIN_Y = 0.f;
static const float MAX_X = CELL_SIZE * GRID_SIZE[0];
static const float MAX_Y = CELL_SIZE * GRID_SIZE[1];
static const int NB_OFF_SCREEN_TILES_MARGIN = 2; // the margin (in number of tiles) that surrounds the on-screen grid with off-screen tiles

static const float REF_SPEED = 5.f * CELL_SIZE; // 5 tiles/sec
static const float REF_SIZE = CELL_SIZE;

static const sf::Time LATEST_VIRUS = sf::seconds(60.f); // latest time at which the virus can arrive
static const sf::Time INCUBATION_TIME = sf::seconds(20.f);
static const sf::Time SICK_TIME = sf::seconds(10.f);

static const int FONT_SIZE = 48;

static const sf::Color TEXT_COLOR(247, 192, 158);
#endif /* constants_hpp */
