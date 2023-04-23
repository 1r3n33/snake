#pragma bank 2

#include <gb/gb.h>

#include "garden.h"

#include "../background.h"
#include "../bonus.h"
#include "../camera.h"
#include "../eyes.h"
#include "../game.h"
#include "../snake.h"
#include "../state.h"
#include "../tiles_copy.h"

#include "../../res/level1_garden/gfx_background.h"
#include "../../res/level1_garden/gfx_garden.h"
#include "../../res/level1_garden/gfx_sprites.h"

void garden_init_background() BANKED
{
    // Load Background tiles and then map
    set_bkg_data(0, gfx_backgroundLen, gfx_background);

    background_init(gfx_garden);
    set_bkg_submap(0U, 0U, DEVICE_SCREEN_WIDTH + 1, DEVICE_SCREEN_HEIGHT + 1, background_get(), BACKGROUND_WIDTH);
    SCX_REG = 0;
    SCY_REG = 0;

    snake_init();

    // Init camera right after the head has been set AND before initializing sprites.
    // Sprites require camera location to be placed at the desired position.
    camera_init(snake_get_head());
}

void garden_init_sprites() BANKED
{
    set_sprite_data(0, gfx_spritesLen, gfx_sprites);

    eyes_init();
    eyes_move(snake_get_head());

    bonus_init();
}

void garden_init() BANKED
{
    HIDE_BKG;
    HIDE_SPRITES;

    state_init();
    tc_init();

    // Init background tiles, snake body and camera
    garden_init_background();

    // Init sprites, eyes and bonus
    garden_init_sprites();

    refresh_OAM();

    SHOW_BKG;
    SHOW_SPRITES;
}

int8_t garden_loop_check() BANKED
{
    State *state = state_get();
    if (state->score == 1)
    {
        return 1;
    }

    return 0;
}

int8_t garden_loop() BANKED
{
    return game_loop(garden_loop_check);
}
