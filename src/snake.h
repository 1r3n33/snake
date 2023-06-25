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

#define SNAKE_STATUS_DISABLED 0U   // No update/tick
#define SNAKE_STATUS_SYNC_FRAME 1U // Transitioning from disable to enable, wait for the correct frame.
#define SNAKE_STATUS_ENABLED 2U    // update/tick enabled

typedef struct Snake
{
    SnakeNode nodes[SNAKE_BUFFER_CAPACITY];
    SnakeNode *head;
    SnakeNode *tail;
    uint8_t status;
    uint8_t tail_locked; // lock tail update (snake grows)
    uint8_t head_locked; // lock head update (snake shrinks)
    uint8_t frame;
} Snake;

void snake_init(uint8_t x, uint8_t y);
void snake_update(uint8_t dir);
void snake_tick(uint8_t frame);
SnakeNode *snake_get_head();
SnakeNode *snake_get_tail();
SnakeNode *snake_advance_head();
SnakeNode *snake_advance_tail();

void snake_lock_tail(uint8_t count); // Lock the tail for 'count' 16-frames cycles.
void snake_lock_head(uint8_t count); // Lock the head for 'count' 16-frames cycles.

void snake_enable_update(uint8_t enabled);

uint8_t snake_length();

#endif // SNAKE_H
