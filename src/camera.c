#include <gb/gb.h>
#include "camera.h"
#include "snake.h"

Camera cam;

// N S W E
const uint8_t CAM_INIT_OFFSET_X[4] = {0U, 0U, 8U, 256U - 8U};
const uint8_t CAM_INIT_OFFSET_Y[4] = {256U - 8U, 8U, 0U, 0U};

void camera_init(SnakeNode *head)
{
    cam.x = (head->x * 8U) + head->offset_x + CAM_INIT_OFFSET_X[head->in];
    cam.y = (head->y * 8U) + head->offset_y + CAM_INIT_OFFSET_Y[head->in];
}

void camera_move(SnakeNode *head)
{
    uint8_t cx = (head->x * 8U) + head->offset_x;
    if (cx > cam.x)
    {
        cam.x++;
    }
    else if (cx < cam.x)
    {
        cam.x--;
    }

    if (cam.x <= (DEVICE_SCREEN_PX_WIDTH / 2U))
    {
        SCX_REG = 0U;
    }
    else if (cam.x > (256U - (DEVICE_SCREEN_PX_WIDTH / 2U)))
    {
        SCX_REG = 256U - DEVICE_SCREEN_PX_WIDTH;
    }
    else
    {
        SCX_REG = cam.x - (DEVICE_SCREEN_PX_WIDTH / 2U);
    }

    uint8_t cy = (head->y * 8U) + head->offset_y;
    if (cy > cam.y)
    {
        cam.y++;
    }
    else if (cy < cam.y)
    {
        cam.y--;
    }

    if (cam.y <= (DEVICE_SCREEN_PX_HEIGHT / 2U))
    {
        SCY_REG = 0U;
    }
    else if (cam.y > (256U - (DEVICE_SCREEN_PX_HEIGHT / 2U)))
    {
        SCY_REG = 256U - DEVICE_SCREEN_PX_HEIGHT;
    }
    else
    {
        SCY_REG = cam.y - (DEVICE_SCREEN_PX_HEIGHT / 2U);
    }
}
