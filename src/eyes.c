#include <gb/gb.h>
#include "camera.h"
#include "eyes.h"
#include "snake.h"

void eyes_init()
{
    set_sprite_tile(0, 24);
    set_sprite_tile(1, 25);
    set_sprite_tile(2, 26);
    set_sprite_tile(3, 27);

    set_sprite_tile(4, 24);
    set_sprite_tile(5, 25);
    set_sprite_tile(6, 26);
    set_sprite_tile(7, 27);
}

#define SUPPRESS_UNDERFLOW(x) ((256U + (x)) & 0xFFU)

// N S W E
const uint8_t SPRITE0_X_OFFSET[4] = {
    SUPPRESS_UNDERFLOW(DEVICE_SPRITE_PX_OFFSET_X - 6U - 8U),
    DEVICE_SPRITE_PX_OFFSET_X + 6U - 8U,
    SUPPRESS_UNDERFLOW(DEVICE_SPRITE_PX_OFFSET_X - 2U - 8U),
    DEVICE_SPRITE_PX_OFFSET_X + 3U - 8U,
};
const uint8_t SPRITE0_Y_OFFSET[4] = {
    DEVICE_SPRITE_PX_OFFSET_Y - 2U - 8U,
    DEVICE_SPRITE_PX_OFFSET_Y + 3U - 8U,
    DEVICE_SPRITE_PX_OFFSET_Y + 6U - 8U,
    DEVICE_SPRITE_PX_OFFSET_Y - 6U - 8U,
};
const uint8_t SPRITE1_X_OFFSET[4] = {
    SUPPRESS_UNDERFLOW(DEVICE_SPRITE_PX_OFFSET_X - 6U - 8U),
    DEVICE_SPRITE_PX_OFFSET_X + 6U - 8U,
    SUPPRESS_UNDERFLOW(DEVICE_SPRITE_PX_OFFSET_X - 2U - 8U),
    DEVICE_SPRITE_PX_OFFSET_X + 3U - 8U,
};
const uint8_t SPRITE1_Y_OFFSET[4] = {
    DEVICE_SPRITE_PX_OFFSET_Y - 2U,
    DEVICE_SPRITE_PX_OFFSET_Y + 3U,
    DEVICE_SPRITE_PX_OFFSET_Y + 6U,
    DEVICE_SPRITE_PX_OFFSET_Y - 6U,
};
const uint8_t SPRITE2_X_OFFSET[4] = {
    DEVICE_SPRITE_PX_OFFSET_X - 6U,
    DEVICE_SPRITE_PX_OFFSET_X + 6U,
    DEVICE_SPRITE_PX_OFFSET_X - 2U,
    DEVICE_SPRITE_PX_OFFSET_X + 3U,
};
const uint8_t SPRITE2_Y_OFFSET[4] = {
    DEVICE_SPRITE_PX_OFFSET_Y - 2U - 8U,
    DEVICE_SPRITE_PX_OFFSET_Y + 3U - 8U,
    DEVICE_SPRITE_PX_OFFSET_Y + 6U - 8U,
    DEVICE_SPRITE_PX_OFFSET_Y - 6U - 8U,
};
const uint8_t SPRITE3_X_OFFSET[4] = {
    DEVICE_SPRITE_PX_OFFSET_X - 6U,
    DEVICE_SPRITE_PX_OFFSET_X + 6U,
    DEVICE_SPRITE_PX_OFFSET_X - 2U,
    DEVICE_SPRITE_PX_OFFSET_X + 3U,
};
const uint8_t SPRITE3_Y_OFFSET[4] = {
    DEVICE_SPRITE_PX_OFFSET_Y - 2U,
    DEVICE_SPRITE_PX_OFFSET_Y + 3U,
    DEVICE_SPRITE_PX_OFFSET_Y + 6U,
    DEVICE_SPRITE_PX_OFFSET_Y - 6U,
};
const uint8_t SPRITE4_X_OFFSET[4] = {
    DEVICE_SPRITE_PX_OFFSET_X + 6U - 8U,
    SUPPRESS_UNDERFLOW(DEVICE_SPRITE_PX_OFFSET_X - 6U - 8U),
    SUPPRESS_UNDERFLOW(DEVICE_SPRITE_PX_OFFSET_X - 2U - 8U),
    DEVICE_SPRITE_PX_OFFSET_X + 3U - 8U,
};
const uint8_t SPRITE4_Y_OFFSET[4] = {
    DEVICE_SPRITE_PX_OFFSET_Y - 2U - 8U,
    DEVICE_SPRITE_PX_OFFSET_Y + 3U - 8U,
    DEVICE_SPRITE_PX_OFFSET_Y - 6U - 8U,
    DEVICE_SPRITE_PX_OFFSET_Y + 6U - 8U,
};
const uint8_t SPRITE5_X_OFFSET[4] = {
    DEVICE_SPRITE_PX_OFFSET_X + 6U - 8U,
    SUPPRESS_UNDERFLOW(DEVICE_SPRITE_PX_OFFSET_X - 6U - 8U),
    SUPPRESS_UNDERFLOW(DEVICE_SPRITE_PX_OFFSET_X - 2U - 8U),
    DEVICE_SPRITE_PX_OFFSET_X + 3U - 8U,
};
const uint8_t SPRITE5_Y_OFFSET[4] = {
    DEVICE_SPRITE_PX_OFFSET_Y - 2U,
    DEVICE_SPRITE_PX_OFFSET_Y + 3U,
    DEVICE_SPRITE_PX_OFFSET_Y - 6U,
    DEVICE_SPRITE_PX_OFFSET_Y + 6U,
};
const uint8_t SPRITE6_X_OFFSET[4] = {
    DEVICE_SPRITE_PX_OFFSET_X + 6U,
    DEVICE_SPRITE_PX_OFFSET_X - 6U,
    DEVICE_SPRITE_PX_OFFSET_X - 2U,
    DEVICE_SPRITE_PX_OFFSET_X + 3U,
};
const uint8_t SPRITE6_Y_OFFSET[4] = {
    DEVICE_SPRITE_PX_OFFSET_Y - 2U - 8U,
    DEVICE_SPRITE_PX_OFFSET_Y + 3U - 8U,
    DEVICE_SPRITE_PX_OFFSET_Y - 6U - 8U,
    DEVICE_SPRITE_PX_OFFSET_Y + 6U - 8U,
};
const uint8_t SPRITE7_X_OFFSET[4] = {
    DEVICE_SPRITE_PX_OFFSET_X + 6U,
    DEVICE_SPRITE_PX_OFFSET_X - 6U,
    DEVICE_SPRITE_PX_OFFSET_X - 2U,
    DEVICE_SPRITE_PX_OFFSET_X + 3U,
};
const uint8_t SPRITE7_Y_OFFSET[4] = {
    DEVICE_SPRITE_PX_OFFSET_Y - 2U,
    DEVICE_SPRITE_PX_OFFSET_Y + 3U,
    DEVICE_SPRITE_PX_OFFSET_Y - 6U,
    DEVICE_SPRITE_PX_OFFSET_Y + 6U,
};

void eyes_move(SnakeNode *head)
{
    Camera *cam = camera_get();
    uint8_t cx = ((head->x * 8U) + head->offset_x) - (cam->sx % 256U);
    uint8_t cy = ((head->y * 8U) + head->offset_y) - (cam->sy % 256U);

    move_sprite(0, cx + SPRITE0_X_OFFSET[head->in], cy + SPRITE0_Y_OFFSET[head->in]);
    move_sprite(1, cx + SPRITE1_X_OFFSET[head->in], cy + SPRITE1_Y_OFFSET[head->in]);
    move_sprite(2, cx + SPRITE2_X_OFFSET[head->in], cy + SPRITE2_Y_OFFSET[head->in]);
    move_sprite(3, cx + SPRITE3_X_OFFSET[head->in], cy + SPRITE3_Y_OFFSET[head->in]);
    move_sprite(4, cx + SPRITE4_X_OFFSET[head->in], cy + SPRITE4_Y_OFFSET[head->in]);
    move_sprite(5, cx + SPRITE5_X_OFFSET[head->in], cy + SPRITE5_Y_OFFSET[head->in]);
    move_sprite(6, cx + SPRITE6_X_OFFSET[head->in], cy + SPRITE6_Y_OFFSET[head->in]);
    move_sprite(7, cx + SPRITE7_X_OFFSET[head->in], cy + SPRITE7_Y_OFFSET[head->in]);
}

void eyes_ko()
{
    set_sprite_tile(0, 28);
    set_sprite_tile(1, 29);
    set_sprite_tile(2, 30);
    set_sprite_tile(3, 31);

    set_sprite_tile(4, 28);
    set_sprite_tile(5, 29);
    set_sprite_tile(6, 30);
    set_sprite_tile(7, 31);
}