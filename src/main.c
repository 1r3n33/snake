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

struct Node
{
    uint8_t x;     // x pos
    uint8_t y;     // y pos
    uint8_t in;    // the enter direction, north, south, east, west
    uint8_t out;   // the exit direction, north, south, east, west
    uint8_t frame; // frame id for animation
};

#define NODE_BUFFER_CAPACITY 32 // must be of power of two

struct Nodes
{
    struct Node buffer[NODE_BUFFER_CAPACITY];
    uint8_t head;
    uint8_t tail;
} nodes;

struct Node *nodes_next_head()
{
    nodes.head = (nodes.head + 1) & 31;
    return nodes.buffer + nodes.head;
}
struct Node *nodes_next_tail()
{
    nodes.tail = (nodes.tail + 1) & 31;
    return nodes.buffer + nodes.tail;
}

void set_bkg(struct Node *node, uint8_t *tiles)
{
    bkg[((node->y + 0) * 32) + (node->x + 0)] = *tiles++;
    bkg[((node->y + 0) * 32) + (node->x + 1)] = *tiles++;
    bkg[((node->y + 1) * 32) + (node->x + 0)] = *tiles++;
    bkg[((node->y + 1) * 32) + (node->x + 1)] = *tiles;
}

void update_nodes(const uint8_t dir) // the new dir
{
    struct Node *head = nodes.buffer + nodes.head;
    struct Node *prev_tail = nodes.buffer + nodes.tail;

    uint8_t empty[4] = {0, 0, 0, 0};
    set_bkg(prev_tail, empty);

    struct Node *next_tail = nodes_next_tail();
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

    if (dir == NODE_DIR_NORTH)
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

        head->out = NODE_DIR_NORTH;

        // new head
        struct Node *new_node = nodes_next_head();
        new_node->x = head->x;
        new_node->y = head->y - 2;
        new_node->in = NODE_DIR_NORTH;
        new_node->out = NODE_DIR_UNKNOWN;
        new_node->frame = 0;

        uint8_t tiles[4] = {8, 10, 9, 11};
        set_bkg(new_node, tiles);
    }
    else if (dir == NODE_DIR_SOUTH)
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

        head->out = NODE_DIR_SOUTH;

        // new head
        struct Node *new_node = nodes_next_head();
        new_node->x = head->x;
        new_node->y = head->y + 2;
        new_node->in = NODE_DIR_SOUTH;
        new_node->out = NODE_DIR_UNKNOWN;
        new_node->frame = 0;

        uint8_t tiles[4] = {16, 18, 17, 19};
        set_bkg(new_node, tiles);
    }
    else if (dir == NODE_DIR_WEST)
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

        head->out = NODE_DIR_WEST;

        // new head
        struct Node *new_node = nodes_next_head();
        new_node->x = head->x - 2;
        new_node->y = head->y;
        new_node->in = NODE_DIR_WEST;
        new_node->out = NODE_DIR_UNKNOWN;
        new_node->frame = 0;
        uint8_t tiles[4] = {20, 22, 21, 23};
        set_bkg(new_node, tiles);
    }
    else if (dir == NODE_DIR_EAST)
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

        head->out = NODE_DIR_EAST;

        // new head
        struct Node *new_node = nodes_next_head();
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
        nodes.tail = 0;
        nodes.head = 5;

        struct Node *node = nodes.buffer + nodes.tail;
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

        struct Node *node = nodes.buffer + nodes.tail;

        uint8_t horiz_tail[4] = {20, 22, 21, 23};
        set_bkg(node, horiz_tail);

        uint8_t horiz_body[4] = {28, 30, 29, 31};
        while (++node < nodes.buffer + nodes.head)
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

    // Loop forever
    while (1)
    {

        // Game main loop processing goes here
        if (frame == 30) // every half sec
        {
            const uint8_t pressed = joypad();
            if (pressed & J_UP)
            {
                update_nodes(NODE_DIR_NORTH);
            }
            else if (pressed & J_DOWN)
            {
                update_nodes(NODE_DIR_SOUTH);
            }
            else if (pressed & J_LEFT)
            {
                update_nodes(NODE_DIR_WEST);
            }
            else if (pressed & J_RIGHT)
            {
                update_nodes(NODE_DIR_EAST);
            }
            frame = 0;
        }

        // Done processing, yield CPU and wait for start of next frame
        wait_vbl_done();
        frame++;
    }
}
