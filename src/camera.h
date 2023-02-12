#ifndef CAMERA_H
#define CAMERA_H

#include <gb/gb.h>
#include "snake.h"

typedef struct Camera
{
    uint8_t x; // target x
    uint8_t y; // target y
} Camera;

void camera_init(SnakeNode *head);
void camera_move(SnakeNode *head);

#endif // CAMERA_H
