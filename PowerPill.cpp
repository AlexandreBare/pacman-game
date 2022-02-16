#include "PowerPill.hpp"
#include "constants.hpp"

static float POWER_PILL_RADIUS = (REF_SIZE - 4.f) / 2.f;
static int POWER_PILL_VALUE = 50;

PowerPill::PowerPill(Coordinates position) : Food(position, POWER_PILL_RADIUS, POWER_PILL_VALUE, true) {}
