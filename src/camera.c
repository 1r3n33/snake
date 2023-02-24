#include <gb/gb.h>
#include "background.h"
#include "camera.h"
#include "snake.h"
#include "tiles_copy.h"

Camera cam;

// N S W E
const int8_t CAM_INIT_OFFSET_X[4] = {0, 0, 8, -8};
const int8_t CAM_INIT_OFFSET_Y[4] = {-8, 8, 0, 0};

Camera *camera_get()
{
    return &cam;
}

void camera_init(SnakeNode *head)
{
    cam.cx = ((uint16_t)head->x * 8U) + head->offset_x + CAM_INIT_OFFSET_X[head->in];
    cam.cy = ((uint16_t)head->y * 8U) + head->offset_y + CAM_INIT_OFFSET_Y[head->in];
    cam.sx = 0;
    cam.sy = 0;
    cam.lrtb[0] = 0;
    cam.lrtb[1] = DEVICE_SCREEN_WIDTH;
    cam.lrtb[2] = 0;
    cam.lrtb[3] = DEVICE_SCREEN_HEIGHT;
}

void camera_move(SnakeNode *head)
{
    uint8_t lrtb[4];

    // Get target
    uint16_t tx = ((uint16_t)head->x * 8U) + head->offset_x;

    // Get direction
    int8_t delta_x = 0;
    if (tx > cam.cx)
    {
        delta_x++;
    }
    else if (tx < cam.cx)
    {
        delta_x--;
    }

    // Move camera center closer to the target
    cam.cx += delta_x;

    // Compute  scroll register value
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

    // Get target
    uint16_t ty = ((uint16_t)head->y * 8U) + head->offset_y;

    // Get direction
    int8_t delta_y = 0;
    if (ty > cam.cy)
    {
        delta_y++;
    }
    else if (ty < cam.cy)
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

void camera_apply()
{
    SCX_REG = cam.sx;
    SCY_REG = cam.sy;
}