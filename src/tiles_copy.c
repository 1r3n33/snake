#include "tiles_copy.h"
#include "background.h"

#define VRAM_ADDRESS 0x9800U

typedef struct Pair
{
    uint8_t src;
    uint16_t dst;
} Pair;

#define SNAKE_COPY_BUFFER_CAPACITY 16U
#define ROW_COPY_BUFFER_CAPACITY (DEVICE_SCREEN_WIDTH + 3U)
#define COLUMN_COPY_BUFFER_CAPACITY (DEVICE_SCREEN_HEIGHT + 3U)

uint8_t tc_snake_count;
Pair tc_snake[SNAKE_COPY_BUFFER_CAPACITY];

uint8_t tc_row_count;
Pair tc_row[ROW_COPY_BUFFER_CAPACITY];

uint8_t tc_col_count;
Pair tc_column[COLUMN_COPY_BUFFER_CAPACITY];

void tc_apply(Pair *p, uint8_t cnt);

void tc_init()
{
    tc_snake_count = 0U;
    tc_row_count = 0U;
    tc_col_count = 0U;
}

void tc_add_snake_tile(uint16_t offset, uint8_t t)
{
    Pair *p = tc_snake + tc_snake_count++;
    p->dst = VRAM_ADDRESS + offset;
    p->src = t;
}

void tc_apply_snake()
{
    tc_apply(tc_snake, tc_snake_count);
    tc_snake_count = 0;
}

void tc_set_row(uint8_t x, uint8_t y, uint8_t cnt)
{
    uint16_t base = VRAM_ADDRESS + ((y % DEVICE_SCREEN_BUFFER_HEIGHT) * DEVICE_SCREEN_BUFFER_WIDTH);
    uint16_t dst = base + (x % DEVICE_SCREEN_BUFFER_WIDTH);
    uint16_t src = (uint16_t)background_get() + (y * BACKGROUND_WIDTH) + x;

    tc_row_count = cnt;

    Pair *p = tc_row;
    while (cnt--)
    {
        p->dst = dst;
        p->src = *(uint8_t *)src;
        dst++;
        dst %= DEVICE_SCREEN_BUFFER_WIDTH;
        dst |= base;
        src++;
        p++;
    }
}

void tc_apply_row()
{
    tc_apply(tc_row, tc_row_count);
    tc_row_count = 0;
}

void tc_set_column(uint8_t x, uint8_t y, uint8_t cnt)
{
    uint16_t dst = VRAM_ADDRESS + ((y % DEVICE_SCREEN_BUFFER_HEIGHT) * DEVICE_SCREEN_BUFFER_WIDTH) + (x % DEVICE_SCREEN_BUFFER_WIDTH);
    uint16_t src = (uint16_t)background_get() + (y * BACKGROUND_WIDTH) + x;

    tc_col_count = cnt;

    Pair *p = tc_column;
    while (cnt--)
    {
        p->dst = dst;
        p->src = *(uint8_t *)src;
        dst += DEVICE_SCREEN_BUFFER_WIDTH;
        dst %= (DEVICE_SCREEN_BUFFER_WIDTH * DEVICE_SCREEN_BUFFER_HEIGHT);
        dst |= VRAM_ADDRESS;
        src += BACKGROUND_WIDTH;
        p++;
    }
}

void tc_apply_column()
{
    tc_apply(tc_column, tc_col_count);
    tc_col_count = 0;
}

void tc_apply(Pair *p, uint8_t cnt)
{
    switch (cnt)
    {
    case 23: // Max of COPY_BUFFER_CAPACITY
        *((uint8_t *)p->dst) = p->src;
        p++;
    case 22:
        *((uint8_t *)p->dst) = p->src;
        p++;
    case 21:
        *((uint8_t *)p->dst) = p->src;
        p++;
    case 20:
        *((uint8_t *)p->dst) = p->src;
        p++;
    case 19:
        *((uint8_t *)p->dst) = p->src;
        p++;
    case 18:
        *((uint8_t *)p->dst) = p->src;
        p++;
    case 17:
        *((uint8_t *)p->dst) = p->src;
        p++;
    case 16:
        *((uint8_t *)p->dst) = p->src;
        p++;
    case 15:
        *((uint8_t *)p->dst) = p->src;
        p++;
    case 14:
        *((uint8_t *)p->dst) = p->src;
        p++;
    case 13:
        *((uint8_t *)p->dst) = p->src;
        p++;
    case 12:
        *((uint8_t *)p->dst) = p->src;
        p++;
    case 11:
        *((uint8_t *)p->dst) = p->src;
        p++;
    case 10:
        *((uint8_t *)p->dst) = p->src;
        p++;
    case 9:
        *((uint8_t *)p->dst) = p->src;
        p++;
    case 8:
        *((uint8_t *)p->dst) = p->src;
        p++;
    case 7:
        *((uint8_t *)p->dst) = p->src;
        p++;
    case 6:
        *((uint8_t *)p->dst) = p->src;
        p++;
    case 5:
        *((uint8_t *)p->dst) = p->src;
        p++;
    case 4:
        *((uint8_t *)p->dst) = p->src;
        p++;
    case 3:
        *((uint8_t *)p->dst) = p->src;
        p++;
    case 2:
        *((uint8_t *)p->dst) = p->src;
        p++;
    case 1:
        *((uint8_t *)p->dst) = p->src;
    }
}
