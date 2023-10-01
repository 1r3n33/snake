#include "tiles_update.h"
#include "camera.h"
#include "background.h"
#include "direction.h"
#include "tiles_copy.h"

//
// Apply a snake tile on both RAM (for collision) and VRAM (for graphics).
//
// - tile_id:
//      Id of the source tile. Can be fully transparent (124).
//      If the source tile has apparent background (>=192) then get an offset from the destination background tile to select the final id to apply.
// - update_bkg:
//      Used to not update RAM for the head. The tile at the head location must not be overwritten.
//      Because we check collisions against head location, we do not want the head to collide with itself.
// - visible:
//      Used to update RAM only (not VRAM) when a tile is not visible.
// - bkg_offset:
//      Distance (in bytes) from the background address (in RAM).
// - vram_offset:
//      Distance (in bytes) from the background address (in VRAM).
//
// Supress 'conditional flow changed by optimizer: so said EVELYN the modified DOG'
#pragma disable_warning 110
//
inline void tu_apply_(uint8_t tile_id, uint8_t update_bkg, uint8_t visible, uint16_t bkg_offset, uint16_t vram_offset)
{
    // Check tile is transparent (Ok to update anyway when the tile is transparent).
    if (tile_id == 124)
    {
        uint8_t source_tile_id = background_update_from_source(bkg_offset);
        if (visible)
            tc_add_snake_tile(vram_offset, source_tile_id);
    }
    else
    {
        // For snake tiles with apparent background (>= 192),
        // get the offset to the real snake tile with corresponding background.
        uint8_t tile_id_offset = tile_id < 192 ? 0 : background_get_snake_tile_offset(bkg_offset);
        uint8_t tile_id_to_apply = tile_id + tile_id_offset;

        if (update_bkg)
            background_update(bkg_offset, tile_id_to_apply);
        if (visible)
            tc_add_snake_tile(vram_offset, tile_id_to_apply);
    }
}

void tu_apply(SnakeNode *node, uint8_t update_bkg)
{
    uint16_t x = node->x;
    uint16_t y = node->y;
    uint16_t bkg_offset = (uint16_t)((y * BACKGROUND_WIDTH) + x);
    uint16_t vram_offset = (uint16_t)(((y % DEVICE_SCREEN_BUFFER_HEIGHT) * DEVICE_SCREEN_BUFFER_WIDTH) + (x % DEVICE_SCREEN_BUFFER_WIDTH));

    tu_apply_(node->tiles[0], update_bkg, 1U, bkg_offset, vram_offset);
    tu_apply_(node->tiles[1], update_bkg, 1U, bkg_offset + 1U, vram_offset + 1U);
    tu_apply_(node->tiles[2], update_bkg, 1U, bkg_offset + BACKGROUND_WIDTH, vram_offset + DEVICE_SCREEN_BUFFER_WIDTH);
    tu_apply_(node->tiles[3], update_bkg, 1U, bkg_offset + BACKGROUND_WIDTH + 1U, vram_offset + DEVICE_SCREEN_BUFFER_WIDTH + 1U);
}

void tu_apply_with_visibility_check(SnakeNode *node, uint8_t update_bkg)
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

    tu_apply_(node->tiles[0], update_bkg, visible, bkg_offset, vram_offset);
    tu_apply_(node->tiles[1], update_bkg, visible, bkg_offset + 1U, vram_offset + 1U);
    tu_apply_(node->tiles[2], update_bkg, visible, bkg_offset + BACKGROUND_WIDTH, vram_offset + DEVICE_SCREEN_BUFFER_WIDTH);
    tu_apply_(node->tiles[3], update_bkg, visible, bkg_offset + BACKGROUND_WIDTH + 1U, vram_offset + DEVICE_SCREEN_BUFFER_WIDTH + 1U);
}

// TODO: Remove dir parameter as it seems to always be node->in
// TODO: Remove update_bkg parameter as it seeems to always be 0
// TODO: Rename tu_apply_head
void tu_apply_with_direction(SnakeNode *node, uint8_t dir, uint8_t update_bkg)
{
    uint16_t x = node->x;
    uint16_t y = node->y;
    uint16_t bkg_offset = (uint16_t)((y * BACKGROUND_WIDTH) + x);
    uint16_t vram_offset = (uint16_t)(((y % DEVICE_SCREEN_BUFFER_HEIGHT) * DEVICE_SCREEN_BUFFER_WIDTH) + (x % DEVICE_SCREEN_BUFFER_WIDTH));

    switch (dir)
    {
    case DIRECTION_NORTH:
        tu_apply_(node->tiles[2], update_bkg, 1U, bkg_offset + BACKGROUND_WIDTH, vram_offset + DEVICE_SCREEN_BUFFER_WIDTH);
        tu_apply_(node->tiles[3], update_bkg, 1U, bkg_offset + BACKGROUND_WIDTH + 1U, vram_offset + DEVICE_SCREEN_BUFFER_WIDTH + 1U);
        break;

    case DIRECTION_SOUTH:
        tu_apply_(node->tiles[0], update_bkg, 1U, bkg_offset, vram_offset);
        tu_apply_(node->tiles[1], update_bkg, 1U, bkg_offset + 1U, vram_offset + 1U);
        break;

    case DIRECTION_WEST:
        tu_apply_(node->tiles[1], update_bkg, 1U, bkg_offset + 1U, vram_offset + 1U);
        tu_apply_(node->tiles[3], update_bkg, 1U, bkg_offset + BACKGROUND_WIDTH + 1U, vram_offset + DEVICE_SCREEN_BUFFER_WIDTH + 1U);
        break;

    case DIRECTION_EAST:
        tu_apply_(node->tiles[0], update_bkg, 1U, bkg_offset, vram_offset);
        tu_apply_(node->tiles[2], update_bkg, 1U, bkg_offset + BACKGROUND_WIDTH, vram_offset + DEVICE_SCREEN_BUFFER_WIDTH);
        break;
    }
}
