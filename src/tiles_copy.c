#include "tiles_copy.h"

#define VRAM_ADDRESS (uint8_t *)(0x9800U)
#define TILES_COPY_CAPACITY 16U

uint8_t *tiles_copy_dst[TILES_COPY_CAPACITY];
uint8_t tiles_copy_values[TILES_COPY_CAPACITY];
uint8_t tiles_copy_count;

void tiles_copy_init()
{
    tiles_copy_count = 0U;
}

void tiles_copy_push(uint16_t offset, uint8_t v)
{
    tiles_copy_dst[tiles_copy_count] = VRAM_ADDRESS + offset;
    tiles_copy_values[tiles_copy_count] = v;
    tiles_copy_count++;
}

void tiles_copy_run()
{
    uint8_t c = tiles_copy_count; // Always a multiple of 2
    uint8_t i = 0;                // Must be traversed in-order to avoid tail overwriting head
    while (i != c)
    {
        **(tiles_copy_dst + i) = *(tiles_copy_values + i);
        ++i;
        **(tiles_copy_dst + i) = *(tiles_copy_values + i);
        ++i;
    }
}
