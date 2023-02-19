#include <gb/gb.h>
#include <rand.h>
#include <stdint.h>
#include <string.h>
#include "background.h"
#include "bonus.h"
#include "camera.h"
#include "direction.h"
#include "eyes.h"
#include "snake.h"
#include "state.h"
#include "tiles_copy.h"
#include "../res/snake_tiles.h"

// Update tile map
// In-memory background (that handle collision) is updated immediately.
// For VRAM update, tiles will be collected with 'tiles_copy_push' and updated during next VBLANK.
// Use mask to specify which tiles of SnakeNode must be updated.
// mask: [bit1][bit2]
//       [bit3][bit4]
void tiles_update(SnakeNode *node, uint8_t mask)
{
    uint16_t offset = (uint16_t)((node->y * 32U) + node->x);

    if (mask & 1)
    {
        background_update(offset, node->tiles[0]);
        tiles_copy_push(offset, node->tiles[0]);
    }
    if (mask & 2)
    {
        background_update(offset + 1, node->tiles[1]);
        tiles_copy_push(offset + 1, node->tiles[1]);
    }
    if (mask & 4)
    {
        background_update(offset + 32, node->tiles[2]);
        tiles_copy_push(offset + 32, node->tiles[2]);
    }
    if (mask & 8)
    {
        background_update(offset + 33, node->tiles[3]);
        tiles_copy_push(offset + 33, node->tiles[3]);
    }
}

const uint8_t snake_tiles_empty[4] = {0, 0, 0, 0};

#define FRAME_COUNT 2

// Heads are expressed in direction
//  __
// (__ <- going W
//
const uint8_t snake_tiles_head_N[FRAME_COUNT * 4] = {0, 0, 4, 6, 4, 6, 8, 9};
const uint8_t snake_tiles_head_S[FRAME_COUNT * 4] = {5, 7, 0, 0, 8, 9, 5, 7};
const uint8_t snake_tiles_head_W[FRAME_COUNT * 4] = {0, 4, 0, 5, 4, 10, 5, 11};
const uint8_t snake_tiles_head_E[FRAME_COUNT * 4] = {6, 0, 7, 0, 10, 6, 11, 7};

// Tails are expressed in direction
//  __
// (__ <- going E
//
const uint8_t snake_tiles_tail_S[FRAME_COUNT * 4] = {4, 6, 8, 9, 0, 0, 4, 6};
const uint8_t snake_tiles_tail_N[FRAME_COUNT * 4] = {8, 9, 5, 7, 5, 7, 0, 0};
const uint8_t snake_tiles_tail_E[FRAME_COUNT * 4] = {4, 10, 5, 11, 0, 4, 0, 5};
const uint8_t snake_tiles_tail_W[FRAME_COUNT * 4] = {10, 6, 11, 7, 6, 0, 7, 0};

// Bodies
//
const uint8_t snake_tiles_body_V[FRAME_COUNT * 4] = {8, 9, 8, 9, 8, 9, 8, 9};
const uint8_t snake_tiles_body_H[FRAME_COUNT * 4] = {10, 10, 11, 11, 10, 10, 11, 11};

// Corners are expressed in direction
//
// | |_
//  \__  <- This corner is both S->E and W->N
// __
// _ \.
//  | |  <- This corner is both N->W and E-S
//
const uint8_t snake_tiles_corner_E_N[FRAME_COUNT * 4] = {2, 18, 23, 19, 2, 18, 23, 19};
const uint8_t snake_tiles_corner_W_N[FRAME_COUNT * 4] = {16, 2, 17, 21, 16, 2, 17, 21};
const uint8_t snake_tiles_corner_E_S[FRAME_COUNT * 4] = {22, 14, 2, 15, 22, 14, 2, 15};
const uint8_t snake_tiles_corner_W_S[FRAME_COUNT * 4] = {12, 20, 13, 2, 12, 20, 13, 2};

const uint8_t snake_tiles_corner_S_W[FRAME_COUNT * 4] = {2, 18, 23, 19, 2, 18, 23, 19};
const uint8_t snake_tiles_corner_S_E[FRAME_COUNT * 4] = {16, 2, 17, 21, 16, 2, 17, 21};
const uint8_t snake_tiles_corner_N_W[FRAME_COUNT * 4] = {22, 14, 2, 15, 22, 14, 2, 15};
const uint8_t snake_tiles_corner_N_E[FRAME_COUNT * 4] = {12, 20, 13, 2, 12, 20, 13, 2};

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
    if (dir != DIRECTION_UNKNOWN && cur_head->in != direction_get_opposite(dir))
    {
        cur_head->out = dir;
    }
    else
    {
        cur_head->out = cur_head->in;
    }

    // Update tail tiles
    if (cur_head->in != DIRECTION_UNKNOWN)
    {
        State *state = state_get();
        SnakeNode *cur_tail = snake_get_tail();

        // Do not shrink if the tail is locked
        if (state->tail_locked)
        {
            // Rewind the tiles pointer so next snake_tick will look the same.
            // Alternatively, It could be implemented in snake_tick to save tiles copy.
            cur_tail->tiles -= 4U;
            // Unlock tail
            state->tail_locked = 0U;
        }
        else
        {

            // Clear the previous tail tile
            cur_tail->tiles = snake_tiles_empty;
            tiles_update(cur_tail, 0xFF);

            // Set the new tail tile
            SnakeNode *new_tail = snake_advance_tail();
            new_tail->tiles = snake_tiles_tail[new_tail->out];
            tiles_update(new_tail, 0xFF);
        }
    }

    // Update head tiles
    if (cur_head->out == DIRECTION_NORTH)
    {
        cur_head->tiles = snake_tiles_dir_north[cur_head->in];
        tiles_update(cur_head, 0xFF);

        SnakeNode *new_head = snake_advance_head();
        new_head->x = cur_head->x;
        new_head->y = cur_head->y - 2;
        new_head->offset_x = 8;
        new_head->offset_y = 15;
        new_head->in = DIRECTION_NORTH;
        new_head->out = DIRECTION_UNKNOWN;
        new_head->tiles = snake_tiles_head_N;
        tiles_update(new_head, 0x0C);
    }
    else if (cur_head->out == DIRECTION_SOUTH)
    {
        cur_head->tiles = snake_tiles_dir_south[cur_head->in];
        tiles_update(cur_head, 0xFF);

        SnakeNode *new_head = snake_advance_head();
        new_head->x = cur_head->x;
        new_head->y = cur_head->y + 2;
        new_head->offset_x = 8;
        new_head->offset_y = 0;
        new_head->in = DIRECTION_SOUTH;
        new_head->out = DIRECTION_UNKNOWN;
        new_head->tiles = snake_tiles_head_S;
        tiles_update(new_head, 0x03);
    }
    else if (cur_head->out == DIRECTION_WEST)
    {
        cur_head->tiles = snake_tiles_dir_west[cur_head->in];
        tiles_update(cur_head, 0xFF);

        SnakeNode *new_head = snake_advance_head();
        new_head->x = cur_head->x - 2;
        new_head->y = cur_head->y;
        new_head->offset_x = 15;
        new_head->offset_y = 8;
        new_head->in = DIRECTION_WEST;
        new_head->out = DIRECTION_UNKNOWN;
        new_head->tiles = snake_tiles_head_W;
        tiles_update(new_head, 0x0A);
    }
    else if (cur_head->out == DIRECTION_EAST)
    {
        cur_head->tiles = snake_tiles_dir_east[cur_head->in];
        tiles_update(cur_head, 0xFF);

        SnakeNode *new_head = snake_advance_head();
        new_head->x = cur_head->x + 2;
        new_head->y = cur_head->y;
        new_head->offset_x = 0;
        new_head->offset_y = 8;
        new_head->in = DIRECTION_EAST;
        new_head->out = DIRECTION_UNKNOWN;
        new_head->tiles = snake_tiles_head_E;
        tiles_update(new_head, 0x05);
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
            wait_vbl_done();
        }
    }

    if (frame == 8)
    {
        tail->tiles += 4;
        tiles_update(tail, 0xFF);

        head->tiles += 4;
        tiles_update(head, 0xFF);
    }
}

void init_bkg_gfx()
{
    // Load Background tiles and then map
    set_bkg_data(0, gfx_snake_tilesLen, gfx_snake_tiles);

    background_init();
    set_bkg_tiles(0, 0, 32u, 32u, background_get());

    // Setup initial snake
    snake_init();

    int x_pos = 2;

    SnakeNode *node = snake_get_head();
    node->x = x_pos;
    node->y = 2;
    node->offset_x = 0;
    node->offset_y = 8;
    node->in = DIRECTION_UNKNOWN;
    node->out = DIRECTION_EAST;
    node->tiles = snake_tiles_tail_E;
    tiles_update(node, 0xFF);

    for (uint8_t i = 0; i < 2; i++)
    {
        node = snake_advance_head();
        x_pos += 2;

        node->x = x_pos;
        node->y = 2;
        node->offset_x = 0;
        node->offset_y = 8;
        node->in = DIRECTION_EAST;
        node->out = DIRECTION_EAST;
        node->tiles = snake_tiles_body_H;
        tiles_update(node, 0xFF);
    }

    node = snake_advance_head();
    x_pos += 2;

    node->x = x_pos;
    node->y = 2;
    node->offset_x = 0;
    node->offset_y = 8;
    node->in = DIRECTION_EAST;
    node->out = DIRECTION_UNKNOWN;
    node->tiles = snake_tiles_head_E;
    tiles_update(node, 0x05);

    // Turn the background map on to make it visible
    SHOW_BKG;
}

void init_sprites_gfx()
{
    set_sprite_data(0, gfx_snake_tilesLen, gfx_snake_tiles);

    eyes_init();
    eyes_move(snake_get_head());

    bonus_init();

    SPRITES_8x8;
    SHOW_SPRITES;
}

void main(void)
{
    state_init();
    tiles_copy_init();

    init_bkg_gfx();
    init_sprites_gfx();

    camera_init(snake_get_head());

    uint8_t frame = 0;
    uint8_t pressedOnce = 0;

    // Loop forever
    while (1)
    {
        // Used as a frame start marker.
        __asm__("halt");

        // Wait for VBLANK to get access to the VRAM.
        while ((STAT_REG & 3) != 1)
            ;

        tiles_copy_run();

        //  Move the camera
        if (pressedOnce)
        {
            camera_move(snake_get_head());
        }

        // Wait for VBLANK to end.
        while ((STAT_REG & 3) == 1)
            ;

        tiles_copy_init(); // reset

        const uint8_t pressed = joypad();
        if (pressedOnce)
        {
            if (frame == 16)
            {
                frame = 0;

                if (pressed & J_UP)
                {
                    snake_update(DIRECTION_NORTH);
                }
                else if (pressed & J_DOWN)
                {
                    snake_update(DIRECTION_SOUTH);
                }
                else if (pressed & J_LEFT)
                {
                    snake_update(DIRECTION_WEST);
                }
                else if (pressed & J_RIGHT)
                {
                    snake_update(DIRECTION_EAST);
                }
                else
                {
                    snake_update(DIRECTION_UNKNOWN);
                }
            }
            else
            {
                snake_tick(frame);
            }

            SnakeNode *head = snake_get_head();
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
    }
}
