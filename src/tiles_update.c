#include "tiles_update.h"
#include "camera.h"
#include "background.h"
#include "direction.h"
#include "tiles_copy.h"

// Supress 'conditional flow changed by optimizer: so said EVELYN the modified DOG'
#pragma disable_warning 110
inline void tu_apply_(uint8_t tile_id, uint8_t visible, uint16_t bkg_offset, uint16_t vram_offset)
{
    // Check tile is transparent
    if (tile_id == 124)
    {
        uint8_t source_tile_id = background_update_from_source(bkg_offset);
        if (visible)
            tc_add_snake_tile(vram_offset, source_tile_id);
    }
    else
    {
        background_update(bkg_offset, tile_id);
        if (visible)
            tc_add_snake_tile(vram_offset, tile_id);
    }
}

void tu_apply(SnakeNode *node)
{
    uint16_t x = node->x;
    uint16_t y = node->y;
    uint16_t bkg_offset = (uint16_t)((y * BACKGROUND_WIDTH) + x);
    uint16_t vram_offset = (uint16_t)(((y % DEVICE_SCREEN_BUFFER_HEIGHT) * DEVICE_SCREEN_BUFFER_WIDTH) + (x % DEVICE_SCREEN_BUFFER_WIDTH));

    tu_apply_(node->tiles[0], 1U, bkg_offset, vram_offset);
    tu_apply_(node->tiles[1], 1U, bkg_offset + 1U, vram_offset + 1U);
    tu_apply_(node->tiles[2], 1U, bkg_offset + BACKGROUND_WIDTH, vram_offset + DEVICE_SCREEN_BUFFER_WIDTH);
    tu_apply_(node->tiles[3], 1U, bkg_offset + BACKGROUND_WIDTH + 1U, vram_offset + DEVICE_SCREEN_BUFFER_WIDTH + 1U);
}

void tu_apply_with_visibility_check(SnakeNode *node)
{
    uint16_t x = node->x;
    uint16_t y = node->y;
    uint16_t bkg_offset = (uint16_t)((y * BACKGROUND_WIDTH) + x);
    uint16_t vram_offset = (uint16_t)(((y % DEVICE_SCREEN_BUFFER_HEIGHT) * DEVICE_SCREEN_BUFFER_WIDTH) + (x % DEVICE_SCREEN_BUFFER_WIDTH));

    Camera *cam = camera_get();
    uint8_t cam_left = cam->lrtb[0];
    uint8_t cam_right = cam->lrtb[1];
    uint8_t cam_top = cam->lrtb[2];
    uint8_t cam_bottom = cam->lrtb[3];

    uint8_t node_left = x;
    uint8_t node_right = x + 2U;
    uint8_t node_top = y;
    uint8_t node_bottom = y + 2U;

    uint8_t visible = (node_left <= cam_right && node_right >= cam_left && node_top <= cam_bottom && node_bottom >= cam_top) ? 1 : 0;

    tu_apply_(node->tiles[0], visible, bkg_offset, vram_offset);
    tu_apply_(node->tiles[1], visible, bkg_offset + 1U, vram_offset + 1U);
    tu_apply_(node->tiles[2], visible, bkg_offset + BACKGROUND_WIDTH, vram_offset + DEVICE_SCREEN_BUFFER_WIDTH);
    tu_apply_(node->tiles[3], visible, bkg_offset + BACKGROUND_WIDTH + 1U, vram_offset + DEVICE_SCREEN_BUFFER_WIDTH + 1U);
}

void tu_apply_with_direction(SnakeNode *node, uint8_t dir)
{
    uint16_t x = node->x;
    uint16_t y = node->y;
    uint16_t bkg_offset = (uint16_t)((y * BACKGROUND_WIDTH) + x);
    uint16_t vram_offset = (uint16_t)(((y % DEVICE_SCREEN_BUFFER_HEIGHT) * DEVICE_SCREEN_BUFFER_WIDTH) + (x % DEVICE_SCREEN_BUFFER_WIDTH));

    switch (dir)
    {
    case DIRECTION_NORTH:
        tu_apply_(node->tiles[2], 1U, bkg_offset + BACKGROUND_WIDTH, vram_offset + DEVICE_SCREEN_BUFFER_WIDTH);
        tu_apply_(node->tiles[3], 1U, bkg_offset + BACKGROUND_WIDTH + 1U, vram_offset + DEVICE_SCREEN_BUFFER_WIDTH + 1U);
        break;

    case DIRECTION_SOUTH:
        tu_apply_(node->tiles[0], 1U, bkg_offset, vram_offset);
        tu_apply_(node->tiles[1], 1U, bkg_offset + 1U, vram_offset + 1U);
        break;

    case DIRECTION_WEST:
        tu_apply_(node->tiles[1], 1U, bkg_offset + 1U, vram_offset + 1U);
        tu_apply_(node->tiles[3], 1U, bkg_offset + BACKGROUND_WIDTH + 1U, vram_offset + DEVICE_SCREEN_BUFFER_WIDTH + 1U);
        break;

    case DIRECTION_EAST:
        tu_apply_(node->tiles[0], 1U, bkg_offset, vram_offset);
        tu_apply_(node->tiles[2], 1U, bkg_offset + BACKGROUND_WIDTH, vram_offset + DEVICE_SCREEN_BUFFER_WIDTH);
        break;
    }
}
