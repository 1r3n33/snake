#include <string.h>
#include "background.h"
#include "direction.h"
#include "../res/gfx_garden.h"

uint8_t background[BACKGROUND_WIDTH * BACKGROUND_HEIGHT];

uint8_t *background_get()
{
    return background;
}

// Initialize a checker background
void background_init()
{
    memcpy(background, gfx_garden, BACKGROUND_WIDTH * BACKGROUND_HEIGHT);
}

void background_update(uint16_t offset, uint8_t v)
{
    *(background + offset) = v;
}

// Tiles with id < 32 are collidable.
uint8_t background_check_collision(SnakeNode *head)
{
    if (head->in == DIRECTION_NORTH)
    {
        uint8_t x = head->x;
        uint8_t y = head->y + (head->offset_y >> 3) - 1;
        uint8_t *dst = background + (y * BACKGROUND_WIDTH) + x;
        return dst[0] ? dst[0] < 32 : dst[1] < 32;
    }
    else if (head->in == DIRECTION_SOUTH)
    {
        uint8_t x = head->x;
        uint8_t y = head->y + (head->offset_y >> 3) + 1;
        uint8_t *dst = background + (y * BACKGROUND_WIDTH) + x;
        return dst[0] ? dst[0] < 32 : dst[1] < 32;
    }
    else if (head->in == DIRECTION_WEST)
    {
        uint8_t x = head->x + (head->offset_x >> 3) - 1;
        uint8_t y = head->y;
        uint8_t *dst = background + (y * BACKGROUND_WIDTH) + x;
        return dst[0] ? dst[0] < 32 : dst[BACKGROUND_WIDTH] < 32;
    }
    else if (head->in == DIRECTION_EAST)
    {
        uint8_t x = head->x + (head->offset_x >> 3) + 1;
        uint8_t y = head->y;
        uint8_t *dst = background + (y * BACKGROUND_WIDTH) + x;
        return dst[0] ? dst[0] < 32 : dst[BACKGROUND_WIDTH] < 32;
    }

    return 0;
}

// Tiles with id < 32 are collidable.
uint8_t background_peek_1x1(uint8_t x, uint8_t y)
{
    return *(background + (y * BACKGROUND_WIDTH) + x) < 32;
}

// Tiles with id < 32 are collidable.
uint8_t background_peek_2x2(uint8_t x, uint8_t y)
{
    uint8_t *bkg = (background + (y * BACKGROUND_WIDTH) + x);
    uint8_t res = *bkg;
    if (res < 32)
        return 1;
    bkg++;
    res = *bkg;
    if (res < 32)
        return 1;
    bkg += BACKGROUND_WIDTH;
    res = *bkg;
    if (res < 32)
        return 1;
    bkg++;
    res = *bkg;
    return res < 32;
}
