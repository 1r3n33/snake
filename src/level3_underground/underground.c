#pragma bank 4
#include <gb/gb.h>
#include "underground.h"
#include "../background.h"
#include "../bonus.h"
#include "../camera.h"
#include "../eyes.h"
#include "../game.h"
#include "../snake.h"
#include "../state.h"
#include "../text.h"
#include "../tiles_copy.h"
#include "../trigger.h"

#include "../../res/level3_underground/underground_tilemap.h"
#include "../../res/level3_underground/underground_tileset.h"

const uint8_t underground_snake_tile_offset[16] = {0, 16, 32, 48, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16, 16};

Trigger trig_underground;

uint8_t fn_underground() BANKED
{
    State *state = state_get();
    if (state->ko == 1)
    {
        return TRIGGER_RESTART_LEVEL;
    }

    SnakeNode *head = snake_get_head();
    if (head->x == 62 && head->y == 8)
    {
        return TRIGGER_NEXT_TRIGGER;
    }

    return TRIGGER_CONTINUE;
}

void underground_init_background() BANKED
{
    // Load Background tiles and then map
    set_bkg_data(0, underground_tilesetLen, underground_tileset);

    background_init(underground_tilemap, underground_snake_tile_offset);
    set_bkg_submap(0U, 0U, DEVICE_SCREEN_WIDTH + 1, DEVICE_SCREEN_HEIGHT + 1, background_get(), BACKGROUND_WIDTH);
    SCX_REG = 0;
    SCY_REG = 0;

    snake_init(4, 4);

    // Init camera right after the head has been set AND before initializing sprites.
    // Sprites require camera location to be placed at the desired position.
    camera_init(snake_get_head());
}

void underground_init_sprites() BANKED
{
    // set_sprite_data(0, gfx_spritesLen, gfx_sprites);

    eyes_init();

    bonus_init();
}

void underground_init() BANKED
{
    HIDE_BKG;
    HIDE_SPRITES;
    HIDE_WIN;

    state_init();
    tc_init();
    text_init();

    trig_underground.check = fn_underground;
    trig_underground.next = 0;
    trigger_init(&trig_underground);

    // Init background tiles and snake body
    underground_init_background();

    // Init sprites, eyes and bonus
    underground_init_sprites();

    refresh_OAM();

    SHOW_BKG;
    SHOW_SPRITES;
}
