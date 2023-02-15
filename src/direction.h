#ifndef DIRECTION_H
#define DIRECTION_H

#include <gb/gb.h>

#define DIRECTION_NORTH 0U
#define DIRECTION_SOUTH 1U
#define DIRECTION_WEST 2U
#define DIRECTION_EAST 3U
#define DIRECTION_UNKNOWN 0xFFU

uint8_t direction_get_opposite(uint8_t dir);

#endif // DIRECTION_H
