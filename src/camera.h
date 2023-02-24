#ifndef CAMERA_H
#define CAMERA_H

#include <gb/gb.h>
#include "snake.h"

typedef struct Camera
{
    uint16_t cx;     // center x
    uint16_t cy;     // center y
    uint16_t sx;     // scroll x
    uint16_t sy;     // scroll y
    uint8_t lrtb[4]; // left, right, top, bottom col/row indices

} Camera;

Camera *camera_get();
void camera_init(SnakeNode *head);
void camera_move(SnakeNode *head);
void camera_apply();

#endif // CAMERA_H
