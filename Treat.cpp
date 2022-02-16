#include "Treat.hpp"
#include "constants.hpp"

static const float TREAT_RADIUS = REF_SIZE / 6.f;
static const int TREAT_VALUE = 10;

Treat::Treat(Coordinates position) : Food(position, TREAT_RADIUS, TREAT_VALUE){}
