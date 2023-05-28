#include <gb/gb.h>
#include "level.h"
#include "trigger.h"

// Supress 'conditional flow changed by optimizer: so said EVELYN the modified DOG'
#pragma disable_warning 110

void main(void)
{
    // Initial hw setup
    ENABLE_RAM;
    SPRITES_8x8;
    HIDE_BKG;
    HIDE_SPRITES;
    HIDE_WIN;

    Level *level = level_init();
    while (1)
    {
        SWITCH_ROM(level->rom_id);
        level->init();

        uint8_t res = level->loop();
        switch (res)
        {
        case TRIGGER_CONTINUE:
        case TRIGGER_NEXT_TRIGGER:
            // Should not be returned!
            break;

        case TRIGGER_NEXT_LEVEL:
            level = level->next;
            break;

        case TRIGGER_RESTART_LEVEL:
            break;
        }
    }
}
