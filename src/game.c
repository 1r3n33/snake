#include <gb/gb.h>
#include <rand.h>
#include "background.h"
#include "bonus.h"
#include "camera.h"
#include "direction.h"
#include "eyes.h"
#include "game.h"
#include "snake.h"
#include "state.h"
#include "tiles_copy.h"
#include "tiles_update.h"

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

void snake_update(const uint8_t dir)
{
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
        tu_apply_with_visibility_check(cur_tail);

        // Set the new tail tile
        SnakeNode *new_tail = snake_advance_tail();
        new_tail->tiles = snake_tiles_tail[new_tail->out];
        tu_apply_with_visibility_check(new_tail);
    }

    // Update head tiles
    if (cur_head->out == DIRECTION_NORTH)
    {
        cur_head->tiles = snake_tiles_dir_north[cur_head->in];
        tu_apply(cur_head);

        SnakeNode *new_head = snake_advance_head();
        new_head->x = cur_head->x;
        new_head->y = cur_head->y - 2;
        new_head->offset_x = 8;
        new_head->offset_y = 15;
        new_head->in = DIRECTION_NORTH;
        new_head->out = DIRECTION_UNKNOWN;
        new_head->tiles = snake_tiles_head_N;
        tu_apply_with_direction(new_head, DIRECTION_NORTH);
    }
    else if (cur_head->out == DIRECTION_SOUTH)
    {
        cur_head->tiles = snake_tiles_dir_south[cur_head->in];
        tu_apply(cur_head);

        SnakeNode *new_head = snake_advance_head();
        new_head->x = cur_head->x;
        new_head->y = cur_head->y + 2;
        new_head->offset_x = 8;
        new_head->offset_y = 0;
        new_head->in = DIRECTION_SOUTH;
        new_head->out = DIRECTION_UNKNOWN;
        new_head->tiles = snake_tiles_head_S;
        tu_apply_with_direction(new_head, DIRECTION_SOUTH);
    }
    else if (cur_head->out == DIRECTION_WEST)
    {
        cur_head->tiles = snake_tiles_dir_west[cur_head->in];
        tu_apply(cur_head);

        SnakeNode *new_head = snake_advance_head();
        new_head->x = cur_head->x - 2;
        new_head->y = cur_head->y;
        new_head->offset_x = 15;
        new_head->offset_y = 8;
        new_head->in = DIRECTION_WEST;
        new_head->out = DIRECTION_UNKNOWN;
        new_head->tiles = snake_tiles_head_W;
        tu_apply_with_direction(new_head, DIRECTION_WEST);
    }
    else if (cur_head->out == DIRECTION_EAST)
    {
        cur_head->tiles = snake_tiles_dir_east[cur_head->in];
        tu_apply(cur_head);

        SnakeNode *new_head = snake_advance_head();
        new_head->x = cur_head->x + 2;
        new_head->y = cur_head->y;
        new_head->offset_x = 0;
        new_head->offset_y = 8;
        new_head->in = DIRECTION_EAST;
        new_head->out = DIRECTION_UNKNOWN;
        new_head->tiles = snake_tiles_head_E;
        tu_apply_with_direction(new_head, DIRECTION_EAST);
    }
}

void snake_tick(uint8_t frame)
{
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

    if (frame == 7 || frame == 15) // frame & 7 == 7
    {
        while (background_check_collision(head))
        {
            eyes_ko();
            NR52_REG = 0;
            NR51_REG = 0;
            NR50_REG = 0;
            wait_vbl_done();
        }
    }

    if (frame == 8)
    {
        tail->tiles += 4;
        tu_apply_with_visibility_check(tail);

        head->tiles += 4;
        tu_apply(head);
    }
}

void vblank_update(uint8_t frame)
{
    // Used as a frame start marker.
    __asm__("halt");

    // Wait for VBLANK to get access to the VRAM.
    while ((STAT_REG & 3) != 1)
        ;

    tc_apply_snake();

    if (frame == 3 || frame == 11)
    {
        tc_apply_row();
    }
    if (frame == 4 || frame == 12)
    {
        tc_apply_column();
    }
    camera_apply();

    // Wait for VBLANK to end.
    while ((STAT_REG & 3) == 1)
        ;
}

int8_t game_loop(int8_t (*loop_check)(void))
{
    uint8_t frame = 0;
    uint8_t pressedOnce = 0;
    uint8_t lastPressed = 0;

    while (1)
    {
        vblank_update(frame);

        const uint8_t pressed = joypad();
        if (pressed)
        {
            lastPressed = pressed;
        }

        if (pressedOnce)
        {
            // At frame 16 (or 0) a new cycle begins.
            // Snake advances using the last pressed direction.
            if (frame == 16)
            {
                frame = 0;

                if (lastPressed & J_UP)
                {
                    snake_update(DIRECTION_NORTH);
                }
                else if (lastPressed & J_DOWN)
                {
                    snake_update(DIRECTION_SOUTH);
                }
                else if (lastPressed & J_LEFT)
                {
                    snake_update(DIRECTION_WEST);
                }
                else if (lastPressed & J_RIGHT)
                {
                    snake_update(DIRECTION_EAST);
                }
            }
            else
            {
                snake_tick(frame);
            }

            SnakeNode *head = snake_get_head();

            camera_move(head);
            eyes_move(head);
            bonus_update(head);

            frame++;
        }
        else
        {
            pressedOnce = pressed & (J_UP | J_DOWN | J_LEFT | J_RIGHT);
            initrand(((uint16_t)DIV_REG << 8) | pressed);
            frame = 1;
        }

        int8_t res = loop_check();
        if (res)
        {
            return res;
        }
    }
}