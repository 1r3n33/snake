#include <gb/gb.h>
#include <stdint.h>
#include <string.h>
#include "../res/snake.h"

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
    uint8_t x;     // x pos
    uint8_t y;     // y pos
    uint8_t in;    // the enter direction, north, south, east, west
    uint8_t out;   // the exit direction, north, south, east, west
    uint8_t frame; // frame id for animation
};

#define NODE_BUFFER_CAPACITY 32 // must be of power of two

struct Snake
{
    struct Node nodes[NODE_BUFFER_CAPACITY];
    uint8_t head;
    uint8_t tail;
} snake;

inline struct Node *snake_get_head()
{
    return snake.nodes + snake.head;
}

inline struct Node *snake_get_tail()
{
    return snake.nodes + snake.head;
}

inline struct Node *snake_advance_head()
{
    snake.head = (snake.head + 1) & 31;
    return snake.nodes + snake.head;
}

inline struct Node *snake_advance_tail()
{
    snake.tail = (snake.tail + 1) & 31;
    return snake.nodes + snake.tail;
}

// set_bkg
//
// Apply tiles at the node location.
// [0][1]
// [2][3]
void set_bkg(const struct Node *const node, const uint8_t *tiles)
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
    *dst = *tiles++;
    dst++;

    // [ ][1]
    // [ ][ ]
    *dst = *tiles++;
    dst += (BKG_WIDTH - 1);

    // [ ][ ]
    // [2][ ]
    *dst = *tiles++;
    dst++;

    // [ ][ ]
    // [ ][3]
    *dst = *tiles;
}

const uint8_t snake_tiles_empty[4] = {0, 0, 0, 0};

// Heads are expressed in direction
//  __
// (__ <- going W
//
const uint8_t snake_tiles_head_N[4] = {8, 10, 9, 11};
const uint8_t snake_tiles_head_S[4] = {16, 18, 17, 19};
const uint8_t snake_tiles_head_W[4] = {20, 22, 21, 23};
const uint8_t snake_tiles_head_E[4] = {12, 14, 13, 15};

// Tails are expressed in direction
//  __
// (__ <- going E
//
const uint8_t snake_tiles_tail_S[4] = {8, 10, 9, 11};
const uint8_t snake_tiles_tail_N[4] = {16, 18, 17, 19};
const uint8_t snake_tiles_tail_E[4] = {20, 22, 21, 23};
const uint8_t snake_tiles_tail_W[4] = {12, 14, 13, 15};

// Bodies
//
const uint8_t snake_tiles_body_V[4] = {24, 26, 25, 27};
const uint8_t snake_tiles_body_H[4] = {28, 30, 29, 31};

// Corners are expressed in direction
//
// | |_
//  \__  <- This corner is both S->E and W->N
// __
// _ \.
//  | |  <- This corner is both N->W and E-S
//
const uint8_t snake_tiles_corner_E_N[4] = {40, 42, 41, 43};
const uint8_t snake_tiles_corner_W_N[4] = {44, 46, 45, 47};
const uint8_t snake_tiles_corner_E_S[4] = {36, 38, 37, 39};
const uint8_t snake_tiles_corner_W_S[4] = {32, 34, 33, 35};

const uint8_t snake_tiles_corner_S_W[4] = {40, 42, 41, 43};
const uint8_t snake_tiles_corner_S_E[4] = {44, 46, 45, 47};
const uint8_t snake_tiles_corner_N_W[4] = {36, 38, 37, 39};
const uint8_t snake_tiles_corner_N_E[4] = {32, 34, 33, 35};

// N S W E
// Indexed by the entering direction of the node
const uint8_t *const snake_tiles_tail[4] = {snake_tiles_tail_N, snake_tiles_tail_S, snake_tiles_tail_W, snake_tiles_tail_E};
const uint8_t *const snake_tiles_dir_north[4] = {snake_tiles_body_V, 0, snake_tiles_corner_W_N, snake_tiles_corner_E_N};
const uint8_t *const snake_tiles_dir_south[4] = {0, snake_tiles_body_V, snake_tiles_corner_W_S, snake_tiles_corner_E_S};
const uint8_t *const snake_tiles_dir_west[4] = {snake_tiles_corner_N_W, snake_tiles_corner_S_W, snake_tiles_body_H, 0};
const uint8_t *const snake_tiles_dir_east[4] = {snake_tiles_corner_N_E, snake_tiles_corner_S_E, 0, snake_tiles_body_H};

void snake_update(const uint8_t dir)
{
    struct Node *const cur_head = snake.nodes + snake.head;
    struct Node *const cur_tail = snake.nodes + snake.tail;

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
        set_bkg(cur_tail, snake_tiles_empty);

        // Set the new tail tile
        struct Node *new_tail = snake_advance_tail();
        set_bkg(new_tail, snake_tiles_tail[new_tail->out]);
    }

    // Update head tiles
    if (cur_head->out == NODE_DIR_NORTH)
    {
        set_bkg(cur_head, snake_tiles_dir_north[cur_head->in]);

        struct Node *new_head = snake_advance_head();
        new_head->x = cur_head->x;
        new_head->y = cur_head->y - 2;
        new_head->in = NODE_DIR_NORTH;
        new_head->out = NODE_DIR_UNKNOWN;
        new_head->frame = 0;
        set_bkg(new_head, snake_tiles_head_N);
    }
    else if (cur_head->out == NODE_DIR_SOUTH)
    {
        set_bkg(cur_head, snake_tiles_dir_south[cur_head->in]);

        struct Node *new_head = snake_advance_head();
        new_head->x = cur_head->x;
        new_head->y = cur_head->y + 2;
        new_head->in = NODE_DIR_SOUTH;
        new_head->out = NODE_DIR_UNKNOWN;
        new_head->frame = 0;
        set_bkg(new_head, snake_tiles_head_S);
    }
    else if (cur_head->out == NODE_DIR_WEST)
    {
        set_bkg(cur_head, snake_tiles_dir_west[cur_head->in]);

        struct Node *new_head = snake_advance_head();
        new_head->x = cur_head->x - 2;
        new_head->y = cur_head->y;
        new_head->in = NODE_DIR_WEST;
        new_head->out = NODE_DIR_UNKNOWN;
        new_head->frame = 0;
        set_bkg(new_head, snake_tiles_head_W);
    }
    else if (cur_head->out == NODE_DIR_EAST)
    {
        set_bkg(cur_head, snake_tiles_dir_east[cur_head->in]);

        struct Node *new_head = snake_advance_head();
        new_head->x = cur_head->x + 2;
        new_head->y = cur_head->y;
        new_head->in = NODE_DIR_EAST;
        new_head->out = NODE_DIR_UNKNOWN;
        new_head->frame = 0;
        set_bkg(new_head, snake_tiles_head_E);
    }

    set_bkg_tiles(0, 0, 32u, 32u, bkg);
}

void init_gfx()
{
    // Load Background tiles and then map
    set_bkg_data(0, 64, gfx_snake);

    // Setup initial snake
    {
        snake.tail = 0;
        snake.head = 5;

        struct Node *node = snake.nodes + snake.tail;
        node->x = 2;
        node->y = 2;
        node->in = NODE_DIR_UNKNOWN;
        node->out = NODE_DIR_EAST;
        node->frame = 0;

        node++;
        node->x = 4;
        node->y = 2;
        node->in = NODE_DIR_EAST;
        node->out = NODE_DIR_EAST;
        node->frame = 0;

        node++;
        node->x = 6;
        node->y = 2;
        node->in = NODE_DIR_EAST;
        node->out = NODE_DIR_EAST;
        node->frame = 0;

        node++;
        node->x = 8;
        node->y = 2;
        node->in = NODE_DIR_EAST;
        node->out = NODE_DIR_EAST;
        node->frame = 0;

        node++;
        node->x = 10;
        node->y = 2;
        node->in = NODE_DIR_EAST;
        node->out = NODE_DIR_EAST;
        node->frame = 0;

        node++;
        node->x = 12;
        node->y = 2;
        node->in = NODE_DIR_EAST;
        node->out = NODE_DIR_UNKNOWN;
        node->frame = 0;
    }

    // Fill bkg
    {
        memset(bkg, 0, 32 * 32);

        struct Node *node = snake.nodes + snake.tail;

        set_bkg(node, snake_tiles_tail_E);

        while (++node < snake.nodes + snake.head)
        {
            set_bkg(node, snake_tiles_body_H);
        }

        set_bkg(node, snake_tiles_head_E);
    }

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
            if (frame == 15)
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
            frame = 14;
        }

        // Done processing, yield CPU and wait for start of next frame
        wait_vbl_done();
        frame++;
    }
}
