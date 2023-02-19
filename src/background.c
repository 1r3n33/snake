#include "background.h"
#include "direction.h"

uint8_t background[BACKGROUND_WIDTH * BACKGROUND_HEIGHT];

uint8_t *background_get()
{
    return background;
}

// Initialize a checker background
void background_init()
{
    for (uint8_t x = 0U; x < BACKGROUND_WIDTH; x++)
    {
        background[0U * BACKGROUND_WIDTH + x] = 2U + ((x ^ 0U) & 1U);
        background[(BACKGROUND_HEIGHT - 1U) * BACKGROUND_WIDTH + x] = 2U + ((x ^ (BACKGROUND_HEIGHT - 1U)) & 1U);
    }

    for (uint8_t y = 1U; y < BACKGROUND_HEIGHT - 1U; y++)
    {
        background[y * BACKGROUND_WIDTH + 0U] = 2U + ((0U ^ y) & 1U);
        background[y * BACKGROUND_WIDTH + (BACKGROUND_WIDTH - 1U)] = 2U + (((BACKGROUND_WIDTH - 1U) ^ y) & 1U);
    }
}

void background_update(uint16_t offset, uint8_t v)
{
    *(background + offset) = v;
}

// Return != 0 on collision.
uint8_t background_check_collision(SnakeNode *head)
{
    if (head->in == DIRECTION_NORTH)
    {
        uint8_t x = head->x;
        uint8_t y = head->y + (head->offset_y >> 3) - 1;
        uint8_t *dst = background + (y * BACKGROUND_WIDTH) + x;
        return dst[0] ? dst[0] : dst[1];
    }
    else if (head->in == DIRECTION_SOUTH)
    {
        uint8_t x = head->x;
        uint8_t y = head->y + (head->offset_y >> 3) + 1;
        uint8_t *dst = background + (y * BACKGROUND_WIDTH) + x;
        return dst[0] ? dst[0] : dst[1];
    }
    else if (head->in == DIRECTION_WEST)
    {
        uint8_t x = head->x + (head->offset_x >> 3) - 1;
        uint8_t y = head->y;
        uint8_t *dst = background + (y * BACKGROUND_WIDTH) + x;
        return dst[0] ? dst[0] : dst[BACKGROUND_WIDTH];
    }
    else if (head->in == DIRECTION_EAST)
    {
        uint8_t x = head->x + (head->offset_x >> 3) + 1;
        uint8_t y = head->y;
        uint8_t *dst = background + (y * BACKGROUND_WIDTH) + x;
        return dst[0] ? dst[0] : dst[BACKGROUND_WIDTH];
    }

    return 0;
}

uint8_t background_peek_1x1(uint8_t x, uint8_t y)
{
    return *(background + (y * BACKGROUND_WIDTH) + x);
}

uint8_t background_peek_2x2(uint8_t x, uint8_t y)
{
    uint8_t *bkg = (background + (y * BACKGROUND_WIDTH) + x);
    uint8_t res = *bkg;
    bkg++;
    res |= *bkg;
    bkg += BACKGROUND_WIDTH;
    res = *bkg;
    bkg++;
    res |= *bkg;
    return res;
}
