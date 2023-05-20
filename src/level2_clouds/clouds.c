#pragma bank 3
#include <gb/gb.h>
#include "clouds.h"
#include "../background.h"
#include "../bonus.h"
#include "../camera.h"
#include "../eyes.h"
#include "../game.h"
#include "../snake.h"
#include "../state.h"
#include "../tiles_copy.h"

#include "../../res/level2_clouds/clouds_tilemap.h"
#include "../../res/level2_clouds/clouds_tileset.h"

const uint8_t clouds_snake_tile_offset[16] = {0, 16, 32, 48, 16, 0, 0, 16, 0, 0, 0, 16, 0, 0, 16, 0};

void clouds_init_background() BANKED
{
    // Load Background tiles and then map
    set_bkg_data(0, clouds_tilesetLen, clouds_tileset);

    background_init(clouds_tilemap, clouds_snake_tile_offset);
    set_bkg_submap(0U, 0U, DEVICE_SCREEN_WIDTH + 1, DEVICE_SCREEN_HEIGHT + 1, background_get(), BACKGROUND_WIDTH);
    SCX_REG = 0;
    SCY_REG = 0;

    snake_init(4, 4);

    // Init camera right after the head has been set AND before initializing sprites.
    // Sprites require camera location to be placed at the desired position.
    camera_init(snake_get_head());
}

void clouds_init_sprites() BANKED
{
    // set_sprite_data(0, gfx_spritesLen, gfx_sprites);

    eyes_init();
    eyes_move(snake_get_head());

    bonus_init();
}

void clouds_init() BANKED
{
    HIDE_BKG;
    HIDE_SPRITES;

    state_init();
    tc_init();

    // Init background tiles and snake body
    clouds_init_background();

    // Init sprites, eyes and bonus
    clouds_init_sprites();

    refresh_OAM();

    SHOW_BKG;
    SHOW_SPRITES;
}

void clouds_loop_update() BANKED
{
}

int8_t clouds_loop_check() BANKED
{
    State *state = state_get();
    if (state->ko == 1)
    {
        return 2;
    }

    if (state->score == 1)
    {
        return 1;
    }

    return 0;
}

int8_t clouds_loop() BANKED
{
    return game_loop(clouds_loop_update, clouds_loop_check);
}
