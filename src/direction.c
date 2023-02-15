#include "direction.h"

uint8_t opposite_direction[4] = {DIRECTION_SOUTH, DIRECTION_NORTH, DIRECTION_EAST, DIRECTION_WEST};

uint8_t direction_get_opposite(uint8_t dir)
{
    return opposite_direction[dir];
}
