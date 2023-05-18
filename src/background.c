#include <string.h>
#include "background.h"
#include "direction.h"

// Tile ids from 128 to 255 are collidable
#define COLLIDABLE_TILE_MASK 0x80U

// Source background in ROM
uint8_t *source;

// Mapping table from background tile to snake tile offset (ROM).
// This gives an offset to snake tiles with white (0), light gray (16), drak gray (32) or black (48) background.
uint8_t *snake_tile_offsets;

// Dynamic background in RAM (display, collision)
uint8_t background[BACKGROUND_WIDTH * BACKGROUND_HEIGHT];

uint8_t *background_get()
{
    return background;
}

void background_init(uint8_t *tilemap, uint8_t *snake_tile_offsets_)
{
    source = tilemap;
    snake_tile_offsets = snake_tile_offsets_;
    memcpy(background, tilemap, BACKGROUND_WIDTH * BACKGROUND_HEIGHT);
}

void background_update(uint16_t offset, uint8_t v)
{
    *(background + offset) = v;
}

uint8_t background_update_from_source(uint16_t offset)
{
    uint8_t source_tile_id = *(source + offset);
    *(background + offset) = source_tile_id;
    return source_tile_id;
}

uint8_t background_get_snake_tile_offset(uint16_t offset)
{
    uint8_t source_tile_id = *(source + offset);
    return snake_tile_offsets[source_tile_id];
}

// TODO: Factorize with background_peek_2x2
uint8_t background_check_collision(SnakeNode *head)
{
    uint8_t x = head->x;
    uint8_t y = head->y;
    uint8_t *dst = background + (y * BACKGROUND_WIDTH) + x;
    return (dst[0] | dst[1] | dst[BACKGROUND_WIDTH] | dst[BACKGROUND_WIDTH + 1]) & COLLIDABLE_TILE_MASK;
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
