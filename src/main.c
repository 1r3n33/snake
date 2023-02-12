#include <gb/gb.h>
#include <stdint.h>
#include <string.h>
#include "camera.h"
#include "eyes.h"
#include "snake.h"
#include "../res/snake_tiles.h"

#define BKG_WIDTH 32
#define BKG_HEIGHT 32
uint8_t bkg[BKG_WIDTH * BKG_HEIGHT];

#define NODE_DIR_NORTH 0
#define NODE_DIR_SOUTH 1
#define NODE_DIR_WEST 2
#define NODE_DIR_EAST 3
#define NODE_DIR_UNKNOWN 0xFF

uint8_t opposite_direction[4] = {NODE_DIR_SOUTH, NODE_DIR_NORTH, NODE_DIR_EAST, NODE_DIR_WEST};

// This is used to refresh tile indices in VRAM.
struct DirtyTile
{
    uint8_t *vram;
    const uint8_t *indices;
};

#define DIRTY_TILES_BUFFER_CAPACITY 4

struct DirtyTiles
{
    struct DirtyTile buffer[DIRTY_TILES_BUFFER_CAPACITY];
    uint8_t count;

} dirty_tiles;

void init_dirty_tiles()
{
    dirty_tiles.count = 0;
}

void add_to_dirty_tiles(SnakeNode *node)
{
    struct DirtyTile *dirty = dirty_tiles.buffer + dirty_tiles.count++;
    dirty->vram = (uint8_t *)(0x9800U + (node->y * 32U) + node->x);
    dirty->indices = node->tiles;
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
    SnakeNode *cur_tail = snake_get_tail();

    // Assign new direction
    if (dir != NODE_DIR_UNKNOWN && cur_head->in != opposite_direction[dir])
    {
        cur_head->out = dir;
    }
    else
    {
        cur_head->out = cur_head->in;
    }

    // Update tail tiles
    if (cur_head->in != NODE_DIR_UNKNOWN)
    {
        // Clear the previous tail tile
        cur_tail->tiles = snake_tiles_empty;
        add_to_dirty_tiles(cur_tail);

        // Set the new tail tile
        SnakeNode *new_tail = snake_advance_tail();
        new_tail->tiles = snake_tiles_tail[new_tail->out];
        add_to_dirty_tiles(new_tail);
    }

    // Update head tiles
    if (cur_head->out == NODE_DIR_NORTH)
    {
        cur_head->tiles = snake_tiles_dir_north[cur_head->in];
        add_to_dirty_tiles(cur_head);

        SnakeNode *new_head = snake_advance_head();
        new_head->x = cur_head->x;
        new_head->y = cur_head->y - 2;
        new_head->offset_x = 8;
        new_head->offset_y = 15;
        new_head->in = NODE_DIR_NORTH;
        new_head->out = NODE_DIR_UNKNOWN;
        new_head->tiles = snake_tiles_head_N;
        add_to_dirty_tiles(new_head);
    }
    else if (cur_head->out == NODE_DIR_SOUTH)
    {
        cur_head->tiles = snake_tiles_dir_south[cur_head->in];
        add_to_dirty_tiles(cur_head);

        SnakeNode *new_head = snake_advance_head();
        new_head->x = cur_head->x;
        new_head->y = cur_head->y + 2;
        new_head->offset_x = 8;
        new_head->offset_y = 0;
        new_head->in = NODE_DIR_SOUTH;
        new_head->out = NODE_DIR_UNKNOWN;
        new_head->tiles = snake_tiles_head_S;
        add_to_dirty_tiles(new_head);
    }
    else if (cur_head->out == NODE_DIR_WEST)
    {
        cur_head->tiles = snake_tiles_dir_west[cur_head->in];
        add_to_dirty_tiles(cur_head);

        SnakeNode *new_head = snake_advance_head();
        new_head->x = cur_head->x - 2;
        new_head->y = cur_head->y;
        new_head->offset_x = 15;
        new_head->offset_y = 8;
        new_head->in = NODE_DIR_WEST;
        new_head->out = NODE_DIR_UNKNOWN;
        new_head->tiles = snake_tiles_head_W;
        add_to_dirty_tiles(new_head);
    }
    else if (cur_head->out == NODE_DIR_EAST)
    {
        cur_head->tiles = snake_tiles_dir_east[cur_head->in];
        add_to_dirty_tiles(cur_head);

        SnakeNode *new_head = snake_advance_head();
        new_head->x = cur_head->x + 2;
        new_head->y = cur_head->y;
        new_head->offset_x = 0;
        new_head->offset_y = 8;
        new_head->in = NODE_DIR_EAST;
        new_head->out = NODE_DIR_UNKNOWN;
        new_head->tiles = snake_tiles_head_E;
        add_to_dirty_tiles(new_head);
    }
}

void snake_tick(uint8_t frame)
{
    SnakeNode *tail = snake_get_tail();
    SnakeNode *head = snake_get_head();

    if (frame == 8)
    {
        tail->tiles += 4;
        add_to_dirty_tiles(tail);

        head->tiles += 4;
        add_to_dirty_tiles(head);
    }

    if (head->in == NODE_DIR_NORTH)
    {
        head->offset_y--;
    }
    else if (head->in == NODE_DIR_SOUTH)
    {
        head->offset_y++;
    }
    else if (head->in == NODE_DIR_WEST)
    {
        head->offset_x--;
    }
    else if (head->in == NODE_DIR_EAST)
    {
        head->offset_x++;
    }
}

void init_bkg_gfx()
{
    // Load Background tiles and then map
    set_bkg_data(0, gfx_snake_tilesLen, gfx_snake_tiles);

    memset(bkg, 0, 32 * 32);
    for (uint8_t i = 0; i < 32; i++)
    {
        bkg[0 * 32 + i] = 2 + (i & 1);
        bkg[31 * 32 + i] = 2 + (i & 1);
        bkg[i * 32 + 0] = 2 + (i & 1);
        bkg[i * 32 + 31] = 2 + (i & 1);
    }
    set_bkg_tiles(0, 0, 32u, 32u, bkg);

    // Setup initial snake
    snake_init();

    int x_pos = 2;

    SnakeNode *node = snake_get_head();
    node->x = x_pos;
    node->y = 2;
    node->offset_x = 0;
    node->offset_y = 8;
    node->in = NODE_DIR_UNKNOWN;
    node->out = NODE_DIR_EAST;
    node->tiles = snake_tiles_tail_E;
    add_to_dirty_tiles(node);

    for (uint8_t i = 0; i < 2; i++)
    {
        node = snake_advance_head();
        x_pos += 2;

        node->x = x_pos;
        node->y = 2;
        node->offset_x = 0;
        node->offset_y = 8;
        node->in = NODE_DIR_EAST;
        node->out = NODE_DIR_EAST;
        node->tiles = snake_tiles_body_H;
        add_to_dirty_tiles(node);
    }

    node = snake_advance_head();
    x_pos += 2;

    node->x = x_pos;
    node->y = 2;
    node->offset_x = 0;
    node->offset_y = 8;
    node->in = NODE_DIR_EAST;
    node->out = NODE_DIR_UNKNOWN;
    node->tiles = snake_tiles_head_E;
    add_to_dirty_tiles(node);

    // Turn the background map on to make it visible
    SHOW_BKG;
}

void init_sprites_gfx()
{
    set_sprite_data(0, gfx_snake_tilesLen, gfx_snake_tiles);

    eyes_init();
    eyes_move(snake_get_head());

    SPRITES_8x8;
    SHOW_SPRITES;
}

void main(void)
{
    init_dirty_tiles();

    init_bkg_gfx();
    init_sprites_gfx();

    camera_init(snake_get_head());

    uint8_t frame = 0;
    uint8_t pressedOnce = 0;

    // Loop forever
    while (1)
    {
        // Wait for VBLANK to get access to the VRAM.
        while ((STAT_REG & 3) != 1)
            ;

        // Copy new tile data
        struct DirtyTile *tile = dirty_tiles.buffer;
        while (tile < (dirty_tiles.buffer + dirty_tiles.count))
        {
            // indices:
            // [0][1]
            // [2][3]

            uint8_t *dst = tile->vram;
            const uint8_t *src = tile->indices;

            *dst = *src++;
            dst++;

            *dst = *src++;
            dst += (BKG_WIDTH - 1);

            *dst = *src++;
            dst++;

            *dst = *src;

            tile++;
        }
        dirty_tiles.count = 0;

        //  Move the camera
        if (pressedOnce)
        {
            camera_move(snake_get_head());
        }

        // Wait for VBLANK to end.
        while ((STAT_REG & 3) == 1)
            ;

        const uint8_t pressed = joypad();
        if (pressedOnce)
        {
            if (frame == 16)
            {
                frame = 0;

                if (pressed & J_UP)
                {
                    snake_update(NODE_DIR_NORTH);
                }
                else if (pressed & J_DOWN)
                {
                    snake_update(NODE_DIR_SOUTH);
                }
                else if (pressed & J_LEFT)
                {
                    snake_update(NODE_DIR_WEST);
                }
                else if (pressed & J_RIGHT)
                {
                    snake_update(NODE_DIR_EAST);
                }
                else
                {
                    snake_update(NODE_DIR_UNKNOWN);
                }
            }
            else
            {
                snake_tick(frame);
            }

            eyes_move(snake_get_head());

            frame++;
        }
        else
        {
            pressedOnce = pressed & (J_UP | J_DOWN | J_LEFT | J_RIGHT);
            frame = 1;
        }
    }
}
