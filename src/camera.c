#include <gb/gb.h>
#include "background.h"
#include "camera.h"
#include "snake.h"
#include "tiles_copy.h"

// Supress 'conditional flow changed by optimizer: so said EVELYN the modified DOG'
#pragma disable_warning 110

#define CAMERA_TARGET_SNAKE 0U
#define CAMERA_TARGET_XY 1U

Camera cam;

// N S W E
const int8_t CAM_INIT_OFFSET_X[4] = {0, 0, 8, -8};
const int8_t CAM_INIT_OFFSET_Y[4] = {-8, 8, 0, 0};

Camera *camera_get()
{
    return &cam;
}

// The camera is centered on the head node
void camera_init()
{
    SnakeNode *head = snake_get_head();

    cam.target = CAMERA_TARGET_SNAKE;

    cam.tx = ((uint16_t)head->x * 8U) + head->offset_x;
    cam.ty = ((uint16_t)head->y * 8U) + head->offset_y;

    // Initial center offset to start by catching up the target
    cam.cx = cam.tx + CAM_INIT_OFFSET_X[head->in];
    cam.cy = cam.ty + CAM_INIT_OFFSET_Y[head->in];

    if (cam.cx < (DEVICE_SCREEN_PX_WIDTH / 2U))
    {
        cam.sx = 0U;
    }
    else if (cam.cx > ((BACKGROUND_WIDTH * 8U) - (DEVICE_SCREEN_PX_WIDTH / 2U)))
    {
        cam.sx = (BACKGROUND_WIDTH * 8U) - DEVICE_SCREEN_PX_WIDTH;
    }
    else
    {
        cam.sx = cam.cx - (DEVICE_SCREEN_PX_WIDTH / 2U);
    }

    if (cam.cy < (DEVICE_SCREEN_PX_HEIGHT / 2U))
    {
        cam.sy = 0U;
    }
    else if (cam.cy > ((BACKGROUND_HEIGHT * 8U) - (DEVICE_SCREEN_PX_HEIGHT / 2U)))
    {
        cam.sy = (BACKGROUND_HEIGHT * 8U) - DEVICE_SCREEN_PX_HEIGHT;
    }
    else
    {
        cam.sy = cam.cy - (DEVICE_SCREEN_PX_HEIGHT / 2U);
    }

    int8_t left = (cam.sx / 8) - 1;
    cam.lrtb[0] = (left < 0) ? 0 : left;
    uint8_t right = ((cam.sx + (DEVICE_SCREEN_WIDTH * 8) - 1) / 8) + 1;
    cam.lrtb[1] = (right > BACKGROUND_WIDTH - 1) ? BACKGROUND_WIDTH - 1 : right;

    int8_t top = (cam.sy / 8) - 1;
    cam.lrtb[2] = (top < 0) ? 0 : top;
    uint8_t bottom = ((cam.sy + (DEVICE_SCREEN_HEIGHT * 8) - 1) / 8) + 1;
    cam.lrtb[3] = (bottom > BACKGROUND_HEIGHT - 1) ? BACKGROUND_HEIGHT - 1 : bottom;

    set_bkg_submap(cam.lrtb[0], cam.lrtb[2], (cam.lrtb[1] - cam.lrtb[0]) + 1U, (cam.lrtb[3] - cam.lrtb[2]) + 1U, background_get(), BACKGROUND_WIDTH);

    SCX_REG = cam.sx;
    SCY_REG = cam.sy;
}

void camera_move_to_target()
{
    uint8_t lrtb[4];

    // Get direction
    int8_t delta_x = 0;
    if (cam.tx > cam.cx)
    {
        delta_x++;
    }
    else if (cam.tx < cam.cx)
    {
        delta_x--;
    }

    // Move camera center closer to the target
    cam.cx += delta_x;

    if (cam.cx < (DEVICE_SCREEN_PX_WIDTH / 2U))
    {
        cam.sx = 0U;
    }
    else if (cam.cx > ((BACKGROUND_WIDTH * 8U) - (DEVICE_SCREEN_PX_WIDTH / 2U)))
    {
        cam.sx = (BACKGROUND_WIDTH * 8U) - DEVICE_SCREEN_PX_WIDTH;
    }
    else
    {
        cam.sx = cam.cx - (DEVICE_SCREEN_PX_WIDTH / 2U);
    }

    int8_t left = (cam.sx / 8) - 1;
    lrtb[0] = (left < 0) ? 0 : left;
    uint8_t right = ((cam.sx + (DEVICE_SCREEN_WIDTH * 8) - 1) / 8) + 1;
    lrtb[1] = (right > BACKGROUND_WIDTH - 1) ? BACKGROUND_WIDTH - 1 : right;

    // Get direction
    int8_t delta_y = 0;
    if (cam.ty > cam.cy)
    {
        delta_y++;
    }
    else if (cam.ty < cam.cy)
    {
        delta_y--;
    }

    // Move camera center closer to the target
    cam.cy += delta_y;

    if (cam.cy < (DEVICE_SCREEN_PX_HEIGHT / 2U))
    {
        cam.sy = 0U;
    }
    else if (cam.cy > ((BACKGROUND_HEIGHT * 8U) - (DEVICE_SCREEN_PX_HEIGHT / 2U)))
    {
        cam.sy = (BACKGROUND_HEIGHT * 8U) - DEVICE_SCREEN_PX_HEIGHT;
    }
    else
    {
        cam.sy = cam.cy - (DEVICE_SCREEN_PX_HEIGHT / 2U);
    }

    int8_t top = (cam.sy / 8) - 1;
    lrtb[2] = (top < 0) ? 0 : top;
    uint8_t bottom = ((cam.sy + (DEVICE_SCREEN_HEIGHT * 8) - 1) / 8) + 1;
    lrtb[3] = (bottom > BACKGROUND_HEIGHT - 1) ? BACKGROUND_HEIGHT - 1 : bottom;

    if (lrtb[0] < cam.lrtb[0])
    {
        tc_set_column(lrtb[0], lrtb[2], (lrtb[3] - lrtb[2]) + 1);
    }

    if (lrtb[1] > cam.lrtb[1])
    {
        tc_set_column(lrtb[1], lrtb[2], (lrtb[3] - lrtb[2]) + 1);
    }

    if (lrtb[2] < cam.lrtb[2])
    {
        tc_set_row(lrtb[0], lrtb[2], (lrtb[1] - lrtb[0]) + 1);
    }

    if (lrtb[3] > cam.lrtb[3])
    {
        tc_set_row(lrtb[0], lrtb[3], (lrtb[1] - lrtb[0]) + 1);
    }

    cam.lrtb[0] = lrtb[0];
    cam.lrtb[1] = lrtb[1];
    cam.lrtb[2] = lrtb[2];
    cam.lrtb[3] = lrtb[3];
}

void camera_move(SnakeNode *head)
{
    switch (cam.target)
    {
    case CAMERA_TARGET_SNAKE:
        cam.tx = ((uint16_t)head->x * 8U) + head->offset_x;
        cam.ty = ((uint16_t)head->y * 8U) + head->offset_y;
        break;

    case CAMERA_TARGET_XY:
        // Must be already set
        break;
    }

    camera_move_to_target();
}

// TODO: Only done in vblank_update. Move it there, remove the function.
void camera_apply()
{
    SCX_REG = cam.sx;
    SCY_REG = cam.sy;
}

void camera_set_snake_target(SnakeNode *head)
{
    cam.target = CAMERA_TARGET_SNAKE;
    cam.tx = ((uint16_t)head->x * 8U) + head->offset_x;
    cam.ty = ((uint16_t)head->y * 8U) + head->offset_y;
}

void camera_set_xy_target(uint16_t tx, uint16_t ty)
{
    cam.target = CAMERA_TARGET_XY;
    cam.tx = tx;
    cam.ty = ty;
}
