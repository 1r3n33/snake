#include "tiles_update.h"
#include "camera.h"
#include "background.h"
#include "direction.h"
#include "tiles_copy.h"

void tu_apply(SnakeNode *node)
{
    uint16_t x = node->x;
    uint16_t y = node->y;
    uint16_t bkg_offset = (uint16_t)((y * BACKGROUND_WIDTH) + x);
    uint16_t vram_offset = (uint16_t)(((y % DEVICE_SCREEN_BUFFER_HEIGHT) * DEVICE_SCREEN_BUFFER_WIDTH) + (x % DEVICE_SCREEN_BUFFER_WIDTH));

    background_update(bkg_offset, node->tiles[0]);
    tiles_copy_push(vram_offset, node->tiles[0]);

    background_update(bkg_offset + 1U, node->tiles[1]);
    tiles_copy_push(vram_offset + 1U, node->tiles[1]);

    background_update(bkg_offset + BACKGROUND_WIDTH, node->tiles[2]);
    tiles_copy_push(vram_offset + DEVICE_SCREEN_BUFFER_WIDTH, node->tiles[2]);

    background_update(bkg_offset + BACKGROUND_WIDTH + 1U, node->tiles[3]);
    tiles_copy_push(vram_offset + DEVICE_SCREEN_BUFFER_WIDTH + 1U, node->tiles[3]);
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

    background_update(bkg_offset, node->tiles[0]);
    if (visible)
        tiles_copy_push(vram_offset, node->tiles[0]);

    background_update(bkg_offset + 1U, node->tiles[1]);
    if (visible)
        tiles_copy_push(vram_offset + 1U, node->tiles[1]);

    background_update(bkg_offset + BACKGROUND_WIDTH, node->tiles[2]);
    if (visible)
        tiles_copy_push(vram_offset + DEVICE_SCREEN_BUFFER_WIDTH, node->tiles[2]);

    background_update(bkg_offset + BACKGROUND_WIDTH + 1U, node->tiles[3]);
    if (visible)
        tiles_copy_push(vram_offset + DEVICE_SCREEN_BUFFER_WIDTH + 1U, node->tiles[3]);
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
        background_update(bkg_offset + BACKGROUND_WIDTH, node->tiles[2]);
        tiles_copy_push(vram_offset + DEVICE_SCREEN_BUFFER_WIDTH, node->tiles[2]);
        background_update(bkg_offset + BACKGROUND_WIDTH + 1U, node->tiles[3]);
        tiles_copy_push(vram_offset + DEVICE_SCREEN_BUFFER_WIDTH + 1U, node->tiles[3]);
        break;

    case DIRECTION_SOUTH:
        background_update(bkg_offset, node->tiles[0]);
        tiles_copy_push(vram_offset, node->tiles[0]);
        background_update(bkg_offset + 1U, node->tiles[1]);
        tiles_copy_push(vram_offset + 1U, node->tiles[1]);
        break;

    case DIRECTION_WEST:
        background_update(bkg_offset + 1U, node->tiles[1]);
        tiles_copy_push(vram_offset + 1U, node->tiles[1]);
        background_update(bkg_offset + BACKGROUND_WIDTH + 1U, node->tiles[3]);
        tiles_copy_push(vram_offset + DEVICE_SCREEN_BUFFER_WIDTH + 1U, node->tiles[3]);
        break;

    case DIRECTION_EAST:
        background_update(bkg_offset, node->tiles[0]);
        tiles_copy_push(vram_offset, node->tiles[0]);
        background_update(bkg_offset + BACKGROUND_WIDTH, node->tiles[2]);
        tiles_copy_push(vram_offset + DEVICE_SCREEN_BUFFER_WIDTH, node->tiles[2]);
        break;
    }
}
