#include <gb/gb.h>
#include "background.h"
#include "direction.h"
#include "eyes.h"
#include "snake.h"
#include "state.h"
#include "tiles_update.h"

Snake snake;

const uint8_t snake_tiles_empty[4] = {124, 124, 124, 124};

#define FRAME_COUNT 2

// Heads are expressed in direction
//  __
// (__ <- going W
//
const uint8_t snake_tiles_head_N[FRAME_COUNT * 4] = {124, 124, 192, 194, 192, 194, 176, 177};
const uint8_t snake_tiles_head_S[FRAME_COUNT * 4] = {193, 195, 124, 124, 176, 177, 193, 195};
const uint8_t snake_tiles_head_W[FRAME_COUNT * 4] = {124, 192, 124, 193, 192, 178, 193, 179};
const uint8_t snake_tiles_head_E[FRAME_COUNT * 4] = {194, 124, 195, 124, 178, 194, 179, 195};

// Tails are expressed in direction
//  __
// (__ <- going E
//
const uint8_t snake_tiles_tail_S[FRAME_COUNT * 4] = {192, 194, 176, 177, 124, 124, 192, 194};
const uint8_t snake_tiles_tail_N[FRAME_COUNT * 4] = {176, 177, 193, 195, 193, 195, 124, 124};
const uint8_t snake_tiles_tail_E[FRAME_COUNT * 4] = {192, 178, 193, 179, 124, 192, 124, 193};
const uint8_t snake_tiles_tail_W[FRAME_COUNT * 4] = {178, 194, 179, 195, 194, 124, 195, 124};

// Bodies
//
const uint8_t snake_tiles_body_V[FRAME_COUNT * 4] = {176, 177, 176, 177, 176, 177, 176, 177};
const uint8_t snake_tiles_body_H[FRAME_COUNT * 4] = {178, 178, 179, 179, 178, 178, 179, 179};

// Corners are expressed in direction
//
// | |_
//  \__  <- This corner is both S->E and W->N
// __
// _ \.
//  | |  <- This corner is both N->W and E-S
//
const uint8_t snake_tiles_corner_E_N[FRAME_COUNT * 4] = {190, 202, 207, 203, 190, 202, 207, 203};
const uint8_t snake_tiles_corner_W_N[FRAME_COUNT * 4] = {200, 190, 201, 205, 200, 190, 201, 205};
const uint8_t snake_tiles_corner_E_S[FRAME_COUNT * 4] = {206, 198, 190, 199, 206, 198, 190, 199};
const uint8_t snake_tiles_corner_W_S[FRAME_COUNT * 4] = {196, 204, 197, 190, 196, 204, 197, 190};

const uint8_t snake_tiles_corner_S_W[FRAME_COUNT * 4] = {190, 202, 207, 203, 190, 202, 207, 203};
const uint8_t snake_tiles_corner_S_E[FRAME_COUNT * 4] = {200, 190, 201, 205, 200, 190, 201, 205};
const uint8_t snake_tiles_corner_N_W[FRAME_COUNT * 4] = {206, 198, 190, 199, 206, 198, 190, 199};
const uint8_t snake_tiles_corner_N_E[FRAME_COUNT * 4] = {196, 204, 197, 190, 196, 204, 197, 190};

// N S W E
// Indexed by the entering direction of the node
const uint8_t *const snake_tiles_tail[4] = {snake_tiles_tail_N, snake_tiles_tail_S, snake_tiles_tail_W, snake_tiles_tail_E};
const uint8_t *const snake_tiles_dir_north[4] = {snake_tiles_body_V, 0, snake_tiles_corner_W_N, snake_tiles_corner_E_N};
const uint8_t *const snake_tiles_dir_south[4] = {0, snake_tiles_body_V, snake_tiles_corner_W_S, snake_tiles_corner_E_S};
const uint8_t *const snake_tiles_dir_west[4] = {snake_tiles_corner_N_W, snake_tiles_corner_S_W, snake_tiles_body_H, 0};
const uint8_t *const snake_tiles_dir_east[4] = {snake_tiles_corner_N_E, snake_tiles_corner_S_E, 0, snake_tiles_body_H};

void snake_init(uint8_t x, uint8_t y)
{
    snake.status = SNAKE_STATUS_ENABLED;
    snake.tail = snake.nodes;
    snake.head = snake.nodes;

    int x_pos = x;

    SnakeNode *node = snake_get_head();
    node->x = x_pos;
    node->y = y;
    node->offset_x = 0;
    node->offset_y = 8;
    node->in = DIRECTION_UNKNOWN;
    node->out = DIRECTION_EAST;
    node->tiles = snake_tiles_tail_E;
    tu_apply(node, 1U);

    for (uint8_t i = 0; i < 2; i++)
    {
        node = snake_advance_head();
        x_pos += 2;

        node->x = x_pos;
        node->y = y;
        node->offset_x = 0;
        node->offset_y = 8;
        node->in = DIRECTION_EAST;
        node->out = DIRECTION_EAST;
        node->tiles = snake_tiles_body_H;
        tu_apply(node, 1U);
    }

    node = snake_advance_head();
    x_pos += 2;

    node->x = x_pos;
    node->y = y;
    node->offset_x = 0;
    node->offset_y = 8;
    node->in = DIRECTION_EAST;
    node->out = DIRECTION_UNKNOWN;
    node->tiles = snake_tiles_head_E;
    tu_apply_with_direction(node, DIRECTION_EAST, 0U);
}

void snake_update(const uint8_t dir)
{
    if (snake.status == SNAKE_STATUS_DISABLED)
    {
        return;
    }

    if (snake.status == SNAKE_STATUS_SYNC_FRAME)
    {
        // Enable if the new frame is the next frame.
        // snake_update always happens at frame 0 of the 16 frames cycle.
        if (snake.frame != 15)
        {
            return;
        }
        else
        {
            snake.status = SNAKE_STATUS_ENABLED;
        }
    }

    // snake_update always happens at frame 0 of the 16 frames cycle.
    snake.frame = 0;

    SnakeNode *cur_head = snake_get_head();

    // Assign new direction
    if (cur_head->in != direction_get_opposite(dir))
    {
        cur_head->out = dir;
    }
    else
    {
        cur_head->out = cur_head->in;
    }

    // Update tail tiles
    State *state = state_get();
    SnakeNode *cur_tail = snake_get_tail();

    // Do not shrink if the tail is locked
    if (state->tail_locked)
    {
        // Rewind the tiles pointer so next snake_tick will look the same.
        // Alternatively, It could be implemented in snake_tick to save tiles copy.
        cur_tail->tiles -= 4U;
        // Unlock tail
        state->tail_locked--;
    }
    else
    {
        // Clear the previous tail tile
        cur_tail->tiles = snake_tiles_empty;
        tu_apply_with_visibility_check(cur_tail, 1U);

        // Set the new tail tile
        SnakeNode *new_tail = snake_advance_tail();
        new_tail->tiles = snake_tiles_tail[new_tail->out];
        tu_apply_with_visibility_check(new_tail, 1U);
    }

    // Update head tiles
    if (cur_head->out == DIRECTION_NORTH)
    {
        cur_head->tiles = snake_tiles_dir_north[cur_head->in];
        tu_apply(cur_head, 1U);

        SnakeNode *new_head = snake_advance_head();
        new_head->x = cur_head->x;
        new_head->y = cur_head->y - 2;
        new_head->offset_x = 8;
        new_head->offset_y = 15;
        new_head->in = DIRECTION_NORTH;
        new_head->out = DIRECTION_UNKNOWN;
        new_head->tiles = snake_tiles_head_N;
        tu_apply_with_direction(new_head, DIRECTION_NORTH, 0U);
    }
    else if (cur_head->out == DIRECTION_SOUTH)
    {
        cur_head->tiles = snake_tiles_dir_south[cur_head->in];
        tu_apply(cur_head, 1U);

        SnakeNode *new_head = snake_advance_head();
        new_head->x = cur_head->x;
        new_head->y = cur_head->y + 2;
        new_head->offset_x = 8;
        new_head->offset_y = 0;
        new_head->in = DIRECTION_SOUTH;
        new_head->out = DIRECTION_UNKNOWN;
        new_head->tiles = snake_tiles_head_S;
        tu_apply_with_direction(new_head, DIRECTION_SOUTH, 0U);
    }
    else if (cur_head->out == DIRECTION_WEST)
    {
        cur_head->tiles = snake_tiles_dir_west[cur_head->in];
        tu_apply(cur_head, 1U);

        SnakeNode *new_head = snake_advance_head();
        new_head->x = cur_head->x - 2;
        new_head->y = cur_head->y;
        new_head->offset_x = 15;
        new_head->offset_y = 8;
        new_head->in = DIRECTION_WEST;
        new_head->out = DIRECTION_UNKNOWN;
        new_head->tiles = snake_tiles_head_W;
        tu_apply_with_direction(new_head, DIRECTION_WEST, 0U);
    }
    else if (cur_head->out == DIRECTION_EAST)
    {
        cur_head->tiles = snake_tiles_dir_east[cur_head->in];
        tu_apply(cur_head, 1U);

        SnakeNode *new_head = snake_advance_head();
        new_head->x = cur_head->x + 2;
        new_head->y = cur_head->y;
        new_head->offset_x = 0;
        new_head->offset_y = 8;
        new_head->in = DIRECTION_EAST;
        new_head->out = DIRECTION_UNKNOWN;
        new_head->tiles = snake_tiles_head_E;
        tu_apply_with_direction(new_head, DIRECTION_EAST, 0U);
    }
}

void snake_tick(uint8_t frame)
{
    if (snake.status == SNAKE_STATUS_DISABLED)
    {
        return;
    }

    if (snake.status == SNAKE_STATUS_SYNC_FRAME)
    {
        // Enable if the new frame is the next frame. 
        if (snake.frame != (frame - 1U))
        {
            return;
        }
        else
        {
            snake.status = SNAKE_STATUS_ENABLED;
        }
    }

    snake.frame = frame;

    SnakeNode *tail = snake_get_tail();
    SnakeNode *head = snake_get_head();

    if (head->in == DIRECTION_NORTH)
    {
        head->offset_y--;
    }
    else if (head->in == DIRECTION_SOUTH)
    {
        head->offset_y++;
    }
    else if (head->in == DIRECTION_WEST)
    {
        head->offset_x--;
    }
    else if (head->in == DIRECTION_EAST)
    {
        head->offset_x++;
    }

    // Check head collision at the beginning of the 16 frames cycle.
    if (frame == 1)
    {
        if (background_check_collision(head))
        {
            State *state = state_get();
            state->ko = 1U;

            eyes_ko();
            NR52_REG = 0;
            NR51_REG = 0;
            NR50_REG = 0;

            while (joypad() != J_START)
            {
                wait_vbl_done();
            }

            return;
        }
    }

    // Update tiles at the middle of the 16 frames cycle.
    if (frame == 8)
    {
        tail->tiles += 4;
        tu_apply_with_visibility_check(tail, 1U);

        head->tiles += 4;
        tu_apply(head, 0U);
    }
}

SnakeNode *snake_get_head()
{
    return snake.head;
}

SnakeNode *snake_get_tail()
{
    return snake.tail;
}

SnakeNode *snake_advance_head()
{
    snake.head++;
    if (snake.head >= (snake.nodes + SNAKE_BUFFER_CAPACITY))
    {
        snake.head = snake.nodes;
    }
    return snake.head;
}

SnakeNode *snake_advance_tail()
{
    snake.tail++;
    if (snake.tail >= (snake.nodes + SNAKE_BUFFER_CAPACITY))
    {
        snake.tail = snake.nodes;
    }
    return snake.tail;
}

void snake_enable_update(uint8_t enabled)
{
    snake.status = enabled ? SNAKE_STATUS_SYNC_FRAME : SNAKE_STATUS_DISABLED;
}
