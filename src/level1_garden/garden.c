#pragma bank 2

#include <gb/gb.h>

#include "garden.h"
#include "mole.h"

#include "../background.h"
#include "../bonus.h"
#include "../camera.h"
#include "../eyes.h"
#include "../game.h"
#include "../snake.h"
#include "../state.h"
#include "../text.h"
#include "../tiles_copy.h"

#include "../../res/level1_garden/gfx_background.h"
#include "../../res/level1_garden/gfx_garden.h"
#include "../../res/level1_garden/gfx_sprites.h"

const uint8_t garden_snake_tile_offset[128] = {0};

const uint8_t text_mole_intro[] = {
    // Hello! My name is
    90, 87, 94, 94, 97, 109, 82, 95, 107, 82, 96, 83, 95, 87, 82, 91, 101, 0,
    // Molly...
    95, 97, 94, 94, 107, 110, 110, 110, 0,
    // abcdefgh
    83, 84, 85, 86, 87, 88, 89, 90, 0,
    // EOF
    0
};

uint8_t garden_player_has_met_mole;

void garden_init_background() BANKED
{
    // Load Background tiles and then map
    set_bkg_data(0, gfx_backgroundLen, gfx_background);

    background_init(gfx_garden, garden_snake_tile_offset);
    set_bkg_submap(0U, 0U, DEVICE_SCREEN_WIDTH + 1, DEVICE_SCREEN_HEIGHT + 1, background_get(), BACKGROUND_WIDTH);
    SCX_REG = 0;
    SCY_REG = 0;

    snake_init(4, 8);

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

    mole_init();
}

void garden_init() BANKED
{
    HIDE_BKG;
    HIDE_SPRITES;
    HIDE_WIN;

    state_init();
    tc_init();
    text_init();

    garden_player_has_met_mole = 0;

    // Init background tiles, snake body and camera
    garden_init_background();

    // Init sprites, eyes and bonus
    garden_init_sprites();

    refresh_OAM();

    SHOW_BKG;
    SHOW_SPRITES;
}

void garden_loop_update() BANKED
{
    mole_update();

    SnakeNode *head = snake_get_head();
    if (garden_player_has_met_mole == 0 && head->x > 53 && head->y > 49)
    {
        text_show(text_mole_intro);
        mole_hide();
        garden_player_has_met_mole = 1;
    }
}

int8_t garden_loop_check() BANKED
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

int8_t garden_loop() BANKED
{
    return game_loop(garden_loop_update, garden_loop_check);
}
