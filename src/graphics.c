#include <gb/gb.h>
#include "graphics.h"
#include "../res/level1_garden/gfx_sprites.h"

void gfx_load_sprites()
{
    uint8_t cur_bank = CURRENT_BANK;
    SWITCH_ROM(2);
    set_sprite_data(0, gfx_spritesLen, gfx_sprites);
    SWITCH_ROM(cur_bank);
}
