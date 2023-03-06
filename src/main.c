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
#include "../res/gfx_sprites.h"
#include "../res/gfx_background.h"

// Update tile map
// In-memory background (that handle collision) is updated immediately.
// For VRAM update, tiles will be collected with 'tiles_copy_push' and updated during next VBLANK.
// Use mask to specify which tiles of SnakeNode must be updated.
// mask: [bit1][bit2]
//       [bit3][bit4]
void tiles_update(SnakeNode *node, uint8_t mask)
{
    uint16_t x = node->x;
    uint16_t y = node->y;
    uint16_t bkg_offset = (uint16_t)((y * BACKGROUND_WIDTH) + x);
    uint16_t vram_offset = (uint16_t)(((y % DEVICE_SCREEN_BUFFER_HEIGHT) * DEVICE_SCREEN_BUFFER_WIDTH) + (x % DEVICE_SCREEN_BUFFER_WIDTH));

    if (mask & 1)
    {
        background_update(bkg_offset, node->tiles[0]);
        tiles_copy_push(vram_offset, node->tiles[0]);
    }
    if (mask & 2)
    {
        background_update(bkg_offset + 1U, node->tiles[1]);
        tiles_copy_push(vram_offset + 1U, node->tiles[1]);
    }
    if (mask & 4)
    {
        background_update(bkg_offset + BACKGROUND_WIDTH, node->tiles[2]);
        tiles_copy_push(vram_offset + DEVICE_SCREEN_BUFFER_WIDTH, node->tiles[2]);
    }
    if (mask & 8)
    {
        background_update(bkg_offset + BACKGROUND_WIDTH + 1U, node->tiles[3]);
        tiles_copy_push(vram_offset + DEVICE_SCREEN_BUFFER_WIDTH + 1U, node->tiles[3]);
    }
}

const uint8_t snake_tiles_empty[4] = {252, 252, 252, 252};

#define FRAME_COUNT 2

// Heads are expressed in direction
//  __
// (__ <- going W
//
const uint8_t snake_tiles_head_N[FRAME_COUNT * 4] = {252, 252, 128, 130, 128, 130, 240, 241};
const uint8_t snake_tiles_head_S[FRAME_COUNT * 4] = {129, 131, 252, 252, 240, 241, 129, 131};
const uint8_t snake_tiles_head_W[FRAME_COUNT * 4] = {252, 128, 252, 129, 128, 242, 129, 243};
const uint8_t snake_tiles_head_E[FRAME_COUNT * 4] = {130, 252, 131, 252, 242, 130, 243, 131};

// Tails are expressed in direction
//  __
// (__ <- going E
//
const uint8_t snake_tiles_tail_S[FRAME_COUNT * 4] = {128, 130, 240, 241, 252, 252, 128, 130};
const uint8_t snake_tiles_tail_N[FRAME_COUNT * 4] = {240, 241, 129, 131, 129, 131, 252, 252};
const uint8_t snake_tiles_tail_E[FRAME_COUNT * 4] = {128, 242, 129, 243, 252, 128, 252, 129};
const uint8_t snake_tiles_tail_W[FRAME_COUNT * 4] = {242, 130, 243, 131, 130, 252, 131, 252};

// Bodies
//
const uint8_t snake_tiles_body_V[FRAME_COUNT * 4] = {240, 241, 240, 241, 240, 241, 240, 241};
const uint8_t snake_tiles_body_H[FRAME_COUNT * 4] = {242, 242, 243, 243, 242, 242, 243, 243};

// Corners are expressed in direction
//
// | |_
//  \__  <- This corner is both S->E and W->N
// __
// _ \.
//  | |  <- This corner is both N->W and E-S
//
const uint8_t snake_tiles_corner_E_N[FRAME_COUNT * 4] = {250, 138, 143, 139, 250, 138, 143, 139};
const uint8_t snake_tiles_corner_W_N[FRAME_COUNT * 4] = {136, 250, 137, 141, 136, 250, 137, 141};
const uint8_t snake_tiles_corner_E_S[FRAME_COUNT * 4] = {142, 134, 250, 135, 142, 134, 250, 135};
const uint8_t snake_tiles_corner_W_S[FRAME_COUNT * 4] = {132, 140, 133, 250, 132, 140, 133, 250};

const uint8_t snake_tiles_corner_S_W[FRAME_COUNT * 4] = {250, 138, 143, 139, 250, 138, 143, 139};
const uint8_t snake_tiles_corner_S_E[FRAME_COUNT * 4] = {136, 250, 137, 141, 136, 250, 137, 141};
const uint8_t snake_tiles_corner_N_W[FRAME_COUNT * 4] = {142, 134, 250, 135, 142, 134, 250, 135};
const uint8_t snake_tiles_corner_N_E[FRAME_COUNT * 4] = {132, 140, 133, 250, 132, 140, 133, 250};

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
    set_bkg_data(0, gfx_backgroundLen, gfx_background);

    background_init();
    set_bkg_submap(0U, 0U, DEVICE_SCREEN_WIDTH + 1, DEVICE_SCREEN_HEIGHT + 1, background_get(), BACKGROUND_WIDTH);

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
    set_sprite_data(0, gfx_spritesLen, gfx_sprites);

    eyes_init();
    eyes_move(snake_get_head());

    bonus_init();

    SPRITES_8x8;
    SHOW_SPRITES;
}

void main(void)
{
    state_init();
    tc_init();

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

        tc_reset();

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
    }
}
