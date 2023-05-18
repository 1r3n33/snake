#include <gb/gb.h>
#include "level.h"

// Supress 'conditional flow changed by optimizer: so said EVELYN the modified DOG'
#pragma disable_warning 110

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
        switch (res)
        {
        case 0:
            // Should never happen because level->loop() should not return on 0.
            break;

        case 1:
            // Go to the next level.
            level = level->next;
            break;

        case 2:
            // Restart the same level.
            break;
        }
    }
}
