#include <gb/gb.h>
#include "level.h"

void main(void)
{
    // Initial hw setup
    ENABLE_RAM;
    SPRITES_8x8;
    HIDE_BKG;
    HIDE_SPRITES;

    Level *level = level_init();
    while (1)
    {
        SWITCH_ROM(level->rom_id);
        level->init();

        int8_t res = level->loop();
        if (res < 0)
        {
            level = level->prev;
        }
        else if (res > 0)
        {
            level = level->next;
        }
    }
}
