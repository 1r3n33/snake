#include <gb/gb.h>
#include "titlescreen.h"

#include "../res/titlescreen_tilemap.h"
#include "../res/titlescreen_tileset.h"

void titlescreen_init()
{
    set_bkg_data(0, TITLESCREEN_TILESET_TILE_COUNT, TITLESCREEN_TILESET);
    set_bkg_tiles(0, 0, TITLESCREEN_TILEMAP_WIDTH, TITLESCREEN_TILEMAP_HEIGHT, TITLESCREEN_TILEMAP);
    SHOW_BKG;
}

void titlescreen_loop()
{
    while (1)
    {
        uint8_t pressed = joypad();
        if (pressed & (J_START | J_A))
        {
            break;
        }

        wait_vbl_done();
    }

    HIDE_BKG;
}
