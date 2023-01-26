#include <gb/gb.h>
#include <stdint.h>
#include <string.h>
#include "../res/snake.h"

uint8_t bkg[32 * 32];

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

void set_bkg(struct Node *node, uint8_t *tiles)
{
    bkg[((node->y + 0) * 32) + (node->x + 0)] = *tiles++;
    bkg[((node->y + 0) * 32) + (node->x + 1)] = *tiles++;
    bkg[((node->y + 1) * 32) + (node->x + 0)] = *tiles++;
    bkg[((node->y + 1) * 32) + (node->x + 1)] = *tiles;
}

void snake_update(const uint8_t dir)
{
    struct Node *const head = snake.nodes + snake.head;
    struct Node *const prev_tail = snake.nodes + snake.tail;

    // Assign new direction.
    if (dir != NODE_DIR_UNKNOWN && head->in != opposite_direction[dir])
    {
        head->out = dir;
    }
    else
    {
        head->out = head->in;
    }

    if (head->in != NODE_DIR_UNKNOWN)
    {
        uint8_t empty[4] = {0, 0, 0, 0};
        set_bkg(prev_tail, empty);

        struct Node *next_tail = snake_advance_tail();
        if (next_tail->out == NODE_DIR_EAST)
        {
            uint8_t tiles[4] = {20, 22, 21, 23};
            set_bkg(next_tail, tiles);
        }
        else if (next_tail->out == NODE_DIR_WEST)
        {
            uint8_t tiles[4] = {12, 14, 13, 15};
            set_bkg(next_tail, tiles);
        }
        else if (next_tail->out == NODE_DIR_NORTH)
        {
            uint8_t tiles[4] = {16, 18, 17, 19};
            set_bkg(next_tail, tiles);
        }
        else if (next_tail->out == NODE_DIR_SOUTH)
        {
            uint8_t tiles[4] = {8, 10, 9, 11};
            set_bkg(next_tail, tiles);
        }
    }

    if (head->out == NODE_DIR_NORTH)
    {
        if (head->in == NODE_DIR_NORTH)
        {
            uint8_t vert_body[4] = {24, 26, 25, 27};
            set_bkg(head, vert_body);
        }
        else if (head->in == NODE_DIR_SOUTH)
        {
            uint8_t vert_body[4] = {24, 26, 25, 27};
            set_bkg(head, vert_body);
        }
        else if (head->in == NODE_DIR_WEST)
        {
            uint8_t corner[4] = {44, 46, 45, 47};
            set_bkg(head, corner);
        }
        else if (head->in == NODE_DIR_EAST)
        {
            uint8_t corner[4] = {40, 42, 41, 43};
            set_bkg(head, corner);
        }

        // new head
        struct Node *new_node = snake_advance_head();
        new_node->x = head->x;
        new_node->y = head->y - 2;
        new_node->in = NODE_DIR_NORTH;
        new_node->out = NODE_DIR_UNKNOWN;
        new_node->frame = 0;

        uint8_t tiles[4] = {8, 10, 9, 11};
        set_bkg(new_node, tiles);
    }
    else if (head->out == NODE_DIR_SOUTH)
    {
        if (head->in == NODE_DIR_NORTH)
        {
            uint8_t vert_body[4] = {24, 26, 25, 27};
            set_bkg(head, vert_body);
        }
        else if (head->in == NODE_DIR_SOUTH)
        {
            uint8_t vert_body[4] = {24, 26, 25, 27};
            set_bkg(head, vert_body);
        }
        else if (head->in == NODE_DIR_WEST)
        {
            uint8_t corner[4] = {32, 34, 33, 35};
            set_bkg(head, corner);
        }
        else if (head->in == NODE_DIR_EAST)
        {
            uint8_t corner[4] = {36, 38, 37, 39};
            set_bkg(head, corner);
        }

        // new head
        struct Node *new_node = snake_advance_head();
        new_node->x = head->x;
        new_node->y = head->y + 2;
        new_node->in = NODE_DIR_SOUTH;
        new_node->out = NODE_DIR_UNKNOWN;
        new_node->frame = 0;

        uint8_t tiles[4] = {16, 18, 17, 19};
        set_bkg(new_node, tiles);
    }
    else if (head->out == NODE_DIR_WEST)
    {
        if (head->in == NODE_DIR_NORTH)
        {
            uint8_t corner[4] = {36, 38, 37, 39};
            set_bkg(head, corner);
        }
        else if (head->in == NODE_DIR_SOUTH)
        {
            uint8_t tiles[4] = {40, 42, 41, 43};
            set_bkg(head, tiles);
        }
        else if (head->in == NODE_DIR_WEST)
        {
            uint8_t tiles[4] = {28, 30, 29, 31};
            set_bkg(head, tiles);
        }
        else if (head->in == NODE_DIR_EAST)
        {
            uint8_t tiles[4] = {28, 30, 29, 31};
            set_bkg(head, tiles);
        }

        // new head
        struct Node *new_node = snake_advance_head();
        new_node->x = head->x - 2;
        new_node->y = head->y;
        new_node->in = NODE_DIR_WEST;
        new_node->out = NODE_DIR_UNKNOWN;
        new_node->frame = 0;
        uint8_t tiles[4] = {20, 22, 21, 23};
        set_bkg(new_node, tiles);
    }
    else if (head->out == NODE_DIR_EAST)
    {
        if (head->in == NODE_DIR_NORTH)
        {
            uint8_t corner[4] = {32, 34, 33, 35};
            set_bkg(head, corner);
        }
        else if (head->in == NODE_DIR_SOUTH)
        {
            uint8_t tiles[4] = {44, 46, 45, 47};
            set_bkg(head, tiles);
        }
        else if (head->in == NODE_DIR_WEST)
        {
            uint8_t tiles[4] = {28, 30, 29, 31};
            set_bkg(head, tiles);
        }
        else if (head->in == NODE_DIR_EAST)
        {
            uint8_t tiles[4] = {28, 30, 29, 31};
            set_bkg(head, tiles);
        }

        // new head
        struct Node *new_node = snake_advance_head();
        new_node->x = head->x + 2;
        new_node->y = head->y;
        new_node->in = NODE_DIR_EAST;
        new_node->out = NODE_DIR_UNKNOWN;
        new_node->frame = 0;
        uint8_t tiles[4] = {12, 14, 13, 15};
        set_bkg(new_node, tiles);
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

        uint8_t horiz_tail[4] = {20, 22, 21, 23};
        set_bkg(node, horiz_tail);

        uint8_t horiz_body[4] = {28, 30, 29, 31};
        while (++node < snake.nodes + snake.head)
        {
            set_bkg(node, horiz_body);
        }

        uint8_t horiz_head[4] = {12, 14, 13, 15};
        set_bkg(node, horiz_head);
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
