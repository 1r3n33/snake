#pragma bank 2
#include <gb/gb.h>
#include "mole.h"
#include "../camera.h"

typedef struct Mole
{
    uint8_t x;
    uint8_t y;
    uint8_t state;
    uint8_t timer;
} Mole;

Mole mole;

const uint8_t mole_tile_ids[2][16] = {
    {0, 65, 71, 0,
     64, 66, 72, 73,
     67, 69, 74, 75,
     68, 70, 77, 76},
    {0, 71, 65, 0,
     73, 72, 66, 64,
     75, 74, 69, 67,
     76, 77, 70, 68}};

void mole_init() BANKED
{
    mole.x = 53;
    mole.y = 52;
    mole.state = 0;
    mole.timer = 255;

    // OBJ ids 12-27
    for (uint8_t i = 0; i < 16; i++)
    {
        set_sprite_tile(12 + i, mole_tile_ids[mole.state][i]);
    }

    // Update to move the sprite to its initial location
    mole_update();
}

void mole_update() BANKED
{
    uint16_t mole_left = mole.x * 8U;
    uint16_t mole_right = mole_left + 64U;
    uint16_t mole_top = mole.y * 8U;
    uint16_t mole_bottom = mole_top + 64U;

    Camera *cam = camera_get();
    uint16_t cam_left = cam->sx;
    uint16_t cam_right = cam_left + DEVICE_SCREEN_PX_WIDTH;
    uint16_t cam_top = cam->sy;
    uint16_t cam_bottom = cam_top + DEVICE_SCREEN_PX_HEIGHT;

    if (mole_left < cam_right && mole_right > cam_left && mole_top < cam_bottom && mole_bottom > cam_top)
    {
        uint8_t x = DEVICE_SPRITE_PX_OFFSET_X + mole_left - (cam->sx % 256U);
        uint8_t y = DEVICE_SPRITE_PX_OFFSET_Y + mole_top - (cam->sy % 256U);
        move_sprite(12, x, y);
        move_sprite(13, x + 8, y);
        move_sprite(14, x + 16, y);
        move_sprite(15, x + 24, y);
        move_sprite(16, x, y + 8);
        move_sprite(17, x + 8, y + 8);
        move_sprite(18, x + 16, y + 8);
        move_sprite(19, x + 24, y + 8);
        move_sprite(20, x, y + 16);
        move_sprite(21, x + 8, y + 16);
        move_sprite(22, x + 16, y + 16);
        move_sprite(23, x + 24, y + 16);
        move_sprite(24, x, y + 24);
        move_sprite(25, x + 8, y + 24);
        move_sprite(26, x + 16, y + 24);
        move_sprite(27, x + 24, y + 24);
    }
    else
    {
        move_sprite(12, 0, 0);
        move_sprite(13, 0, 0);
        move_sprite(14, 0, 0);
        move_sprite(15, 0, 0);
        move_sprite(16, 0, 0);
        move_sprite(17, 0, 0);
        move_sprite(18, 0, 0);
        move_sprite(19, 0, 0);
        move_sprite(20, 0, 0);
        move_sprite(21, 0, 0);
        move_sprite(22, 0, 0);
        move_sprite(23, 0, 0);
        move_sprite(24, 0, 0);
        move_sprite(25, 0, 0);
        move_sprite(26, 0, 0);
        move_sprite(27, 0, 0);
    }
}
