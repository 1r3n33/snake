#include <string.h>
#include "background.h"
#include "direction.h"

// Tile ids from 128 to 255 are collidable
#define COLLIDABLE_TILE_MASK 0x80U

uint8_t background[BACKGROUND_WIDTH * BACKGROUND_HEIGHT];

uint8_t *background_get()
{
    return background;
}

void background_init(uint8_t *tilemap)
{
    memcpy(background, tilemap, BACKGROUND_WIDTH * BACKGROUND_HEIGHT);
}

void background_update(uint16_t offset, uint8_t v)
{
    *(background + offset) = v;
}

uint8_t background_check_collision(SnakeNode *head)
{
    if (head->in == DIRECTION_NORTH)
    {
        uint8_t x = head->x;
        uint8_t y = head->y + (head->offset_y >> 3) - 1;
        uint8_t *dst = background + (y * BACKGROUND_WIDTH) + x;
        return (dst[0] | dst[1]) & COLLIDABLE_TILE_MASK;
    }
    else if (head->in == DIRECTION_SOUTH)
    {
        uint8_t x = head->x;
        uint8_t y = head->y + (head->offset_y >> 3) + 1;
        uint8_t *dst = background + (y * BACKGROUND_WIDTH) + x;
        return (dst[0] | dst[1]) & COLLIDABLE_TILE_MASK;
    }
    else if (head->in == DIRECTION_WEST)
    {
        uint8_t x = head->x + (head->offset_x >> 3) - 1;
        uint8_t y = head->y;
        uint8_t *dst = background + (y * BACKGROUND_WIDTH) + x;
        return (dst[0] | dst[BACKGROUND_WIDTH]) & COLLIDABLE_TILE_MASK;
    }
    else if (head->in == DIRECTION_EAST)
    {
        uint8_t x = head->x + (head->offset_x >> 3) + 1;
        uint8_t y = head->y;
        uint8_t *dst = background + (y * BACKGROUND_WIDTH) + x;
        return (dst[0] | dst[BACKGROUND_WIDTH]) & COLLIDABLE_TILE_MASK;
    }

    return 0;
}

uint8_t background_peek_1x1(uint8_t x, uint8_t y)
{
    return *(background + (y * BACKGROUND_WIDTH) + x) & COLLIDABLE_TILE_MASK;
}

uint8_t background_peek_2x2(uint8_t x, uint8_t y)
{
    uint8_t *bkg = (background + (y * BACKGROUND_WIDTH) + x);
    uint8_t res = *bkg;

    bkg++;
    res |= *bkg;

    bkg += BACKGROUND_WIDTH;
    res |= *bkg;

    bkg++;
    res |= *bkg;

    return res & COLLIDABLE_TILE_MASK;
}
