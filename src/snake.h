#ifndef SNAKE_H
#define SNAKE_H

#include <gb/gb.h>

typedef struct SnakeNode
{
    uint8_t x;            // x pos
    uint8_t y;            // y pos
    uint8_t offset_x;     // center x offset
    uint8_t offset_y;     // center y offset
    uint8_t in;           // the enter direction, north, south, east, west
    uint8_t out;          // the exit direction, north, south, east, west
    const uint8_t *tiles; // tile indices
} SnakeNode;

#define SNAKE_BUFFER_CAPACITY 64 // must be of power of two

typedef struct Snake
{
    SnakeNode nodes[SNAKE_BUFFER_CAPACITY];
    SnakeNode *head;
    SnakeNode *tail;
} Snake;

void snake_init(uint8_t x, uint8_t y);
void snake_update(uint8_t dir);
void snake_tick(uint8_t frame);
SnakeNode *snake_get_head();
SnakeNode *snake_get_tail();
SnakeNode *snake_advance_head();
SnakeNode *snake_advance_tail();

#endif // SNAKE_H
