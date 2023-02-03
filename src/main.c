#include <gb/gb.h>
#include <stdint.h>
#include <string.h>
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

struct Node
{
    uint8_t x;            // x pos
    uint8_t y;            // y pos
    uint8_t in;           // the enter direction, north, south, east, west
    uint8_t out;          // the exit direction, north, south, east, west
    const uint8_t *tiles; // tile indices
};

#define NODE_BUFFER_CAPACITY 16 // must be of power of two

struct Snake
{
    struct Node nodes[NODE_BUFFER_CAPACITY];
    struct Node *head;
    struct Node *tail;
} snake;

inline struct Node *snake_advance_head()
{
    snake.head++;
    if (snake.head >= (snake.nodes + NODE_BUFFER_CAPACITY))
    {
        snake.head = snake.nodes;
    }
    return snake.head;
}

inline struct Node *snake_advance_tail()
{
    snake.tail++;
    if (snake.tail >= (snake.nodes + NODE_BUFFER_CAPACITY))
    {
        snake.tail = snake.nodes;
    }
    return snake.tail;
}

// set_bkg
//
// Apply tiles at the node location.
// [0][1]
// [2][3]
void bkg_apply_node(struct Node *const node)
{
    /* Original code
    bkg[((node->y + 0) * 32) + (node->x + 0)] = *tiles++;
    bkg[((node->y + 0) * 32) + (node->x + 1)] = *tiles++;
    bkg[((node->y + 1) * 32) + (node->x + 0)] = *tiles++;
    bkg[((node->y + 1) * 32) + (node->x + 1)] = *tiles;
    */

    // Set base pointer
    uint8_t *dst = bkg + (node->y * BKG_WIDTH) + node->x;

    // [0][ ]
    // [ ][ ]
    *dst = *node->tiles++;
    dst++;

    // [ ][1]
    // [ ][ ]
    *dst = *node->tiles++;
    dst += (BKG_WIDTH - 1);

    // [ ][ ]
    // [2][ ]
    *dst = *node->tiles++;
    dst++;

    // [ ][ ]
    // [ ][3]
    *dst = *node->tiles++;
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
    struct Node *const cur_head = snake.head;
    struct Node *const cur_tail = snake.tail;

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
        bkg_apply_node(cur_tail);

        // Set the new tail tile
        struct Node *new_tail = snake_advance_tail();
        new_tail->tiles = snake_tiles_tail[new_tail->out];
        bkg_apply_node(new_tail);
    }

    // Update head tiles
    if (cur_head->out == NODE_DIR_NORTH)
    {
        cur_head->tiles = snake_tiles_dir_north[cur_head->in];
        bkg_apply_node(cur_head);

        struct Node *new_head = snake_advance_head();
        new_head->x = cur_head->x;
        new_head->y = cur_head->y - 2;
        new_head->in = NODE_DIR_NORTH;
        new_head->out = NODE_DIR_UNKNOWN;
        new_head->tiles = snake_tiles_head_N;
        bkg_apply_node(new_head);
    }
    else if (cur_head->out == NODE_DIR_SOUTH)
    {
        cur_head->tiles = snake_tiles_dir_south[cur_head->in];
        bkg_apply_node(cur_head);

        struct Node *new_head = snake_advance_head();
        new_head->x = cur_head->x;
        new_head->y = cur_head->y + 2;
        new_head->in = NODE_DIR_SOUTH;
        new_head->out = NODE_DIR_UNKNOWN;
        new_head->tiles = snake_tiles_head_S;
        bkg_apply_node(new_head);
    }
    else if (cur_head->out == NODE_DIR_WEST)
    {
        cur_head->tiles = snake_tiles_dir_west[cur_head->in];
        bkg_apply_node(cur_head);

        struct Node *new_head = snake_advance_head();
        new_head->x = cur_head->x - 2;
        new_head->y = cur_head->y;
        new_head->in = NODE_DIR_WEST;
        new_head->out = NODE_DIR_UNKNOWN;
        new_head->tiles = snake_tiles_head_W;
        bkg_apply_node(new_head);
    }
    else if (cur_head->out == NODE_DIR_EAST)
    {
        cur_head->tiles = snake_tiles_dir_east[cur_head->in];
        bkg_apply_node(cur_head);

        struct Node *new_head = snake_advance_head();
        new_head->x = cur_head->x + 2;
        new_head->y = cur_head->y;
        new_head->in = NODE_DIR_EAST;
        new_head->out = NODE_DIR_UNKNOWN;
        new_head->tiles = snake_tiles_head_E;
        bkg_apply_node(new_head);
    }

    set_bkg_tiles(0, 0, 32u, 32u, bkg);
}

void snake_tick()
{
    bkg_apply_node(snake.tail);
    bkg_apply_node(snake.head);

    set_bkg_tiles(0, 0, 32u, 32u, bkg);
}

void init_gfx()
{
    // Load Background tiles and then map
    set_bkg_data(0, gfx_snake_tilesLen, gfx_snake_tiles);

    memset(bkg, 0, 32 * 32);

    // Setup initial snake
    snake.tail = snake.nodes;
    snake.head = snake.nodes + 6;

    int x_pos = 2;

    struct Node *node = snake.tail;
    node->x = x_pos;
    node->y = 2;
    node->in = NODE_DIR_UNKNOWN;
    node->out = NODE_DIR_EAST;
    node->tiles = snake_tiles_tail_E;
    bkg_apply_node(node);

    while (node < snake.head - 1)
    {
        node++;
        x_pos += 2;

        node->x = x_pos;
        node->y = 2;
        node->in = NODE_DIR_EAST;
        node->out = NODE_DIR_EAST;
        node->tiles = snake_tiles_body_H;
        bkg_apply_node(node);
    }

    node++;
    x_pos += 2;

    node->x = x_pos;
    node->y = 2;
    node->in = NODE_DIR_EAST;
    node->out = NODE_DIR_UNKNOWN;
    node->tiles = snake_tiles_head_E;
    bkg_apply_node(node);

    set_bkg_tiles(0, 0, 32u, 32u, bkg);

    // Turn the background map on to make it visible
    SHOW_BKG;
}

void main(void)
{
    init_gfx();

    int8_t frame = 0;
    int8_t pressedOnce = 0;

    // Loop forever
    while (1)
    {
        const uint8_t pressed = joypad();
        if (pressedOnce)
        {
            if (frame == 8)
            {
                snake_tick();
            }
            else if (frame == 16)
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
        }
        else
        {
            pressedOnce = pressed & (J_UP | J_DOWN | J_LEFT | J_RIGHT);
            frame = 7;
        }

        // Done processing, yield CPU and wait for start of next frame
        wait_vbl_done();
        frame++;
    }
}
