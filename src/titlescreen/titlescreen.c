#include <gb/gb.h>
#include "titlescreen.h"

#include "../state.h"
#include "../tiles_copy.h"
#include "../trigger.h"

#include "../../ext/hUGEDriver/include/hUGEDriver.h"

#include "../../res/titlescreen/titlescreen_tilemap.h"
#include "../../res/titlescreen/titlescreen_tileset.h"

extern const hUGESong_t sample_song;

void titlescreen_init() BANKED
{
    HIDE_BKG;
    HIDE_SPRITES;

    state_init();
    tc_init();

    set_bkg_data(0, TITLESCREEN_TILESET_TILE_COUNT, TITLESCREEN_TILESET);
    set_bkg_tiles(0, 0, TITLESCREEN_TILEMAP_WIDTH, TITLESCREEN_TILEMAP_HEIGHT, TITLESCREEN_TILEMAP);
    SCX_REG = 0;
    SCY_REG = 0;

    NR52_REG = 0x80;
    NR51_REG = 0xFF;
    NR50_REG = 0x77;

    hUGE_init(&sample_song);

    SHOW_BKG;
}

uint8_t titlescreen_loop() BANKED
{
    int8_t res = TRIGGER_CONTINUE;

    while (1)
    {
        uint8_t pressed = joypad();
        if (pressed & (J_START | J_A))
        {
            res = TRIGGER_NEXT_LEVEL;
            break;
        }

        wait_vbl_done();
        hUGE_dosound();
    }

    HIDE_BKG;
    NR52_REG = 0;
    NR51_REG = 0;
    NR50_REG = 0;

    return res;
}
