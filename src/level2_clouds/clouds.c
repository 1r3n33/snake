#pragma bank 3
#include <gb/gb.h>
#include "clouds.h"
#include "../background.h"
#include "../bonus.h"
#include "../camera.h"
#include "../direction.h"
#include "../eyes.h"
#include "../game.h"
#include "../graphics.h"
#include "../projectile.h"
#include "../snake.h"
#include "../state.h"
#include "../text.h"
#include "../tiles_copy.h"
#include "../trigger.h"

#include "../../res/level2_clouds/clouds_tilemap.h"
#include "../../res/level2_clouds/clouds_tileset.h"

const uint8_t clouds_snake_tile_offset[16] = {
    GFX_SNAKE_OFFSET_0, GFX_SNAKE_OFFSET_1, GFX_SNAKE_OFFSET_2, GFX_SNAKE_OFFSET_3,
    GFX_SNAKE_OFFSET_1, GFX_SNAKE_OFFSET_0, GFX_SNAKE_OFFSET_0, GFX_SNAKE_OFFSET_1,
    GFX_SNAKE_OFFSET_0, GFX_SNAKE_OFFSET_0, GFX_SNAKE_OFFSET_0, GFX_SNAKE_OFFSET_1,
    GFX_SNAKE_OFFSET_0, GFX_SNAKE_OFFSET_0, GFX_SNAKE_OFFSET_1, GFX_SNAKE_OFFSET_0};

Trigger trig_clouds;

uint8_t fn_clouds() BANKED
{
    State *state = state_get();
    if (state->ko == 1)
    {
        return TRIGGER_RESTART_LEVEL;
    }

    if (state->score == 1)
    {
        return TRIGGER_NEXT_LEVEL;
    }

    return TRIGGER_CONTINUE;
}

void clouds_init_background() BANKED
{
    // Load Background tiles and then map
    set_bkg_data(0, clouds_tilesetLen, clouds_tileset);

    background_init(clouds_tilemap, clouds_snake_tile_offset);

    const uint8_t dirs[] = {DIRECTION_EAST, DIRECTION_SOUTH, DIRECTION_EAST, DIRECTION_NORTH, DIRECTION_NORTH, DIRECTION_EAST, DIRECTION_SOUTH, DIRECTION_EAST, DIRECTION_EAST};
    snake_init(4, 8, dirs, sizeof(dirs));

    // Init camera right after the head has been set AND before initializing sprites.
    // Sprites require camera location to be placed at the desired position.
    camera_init();
}

void clouds_init_sprites() BANKED
{
    gfx_load_sprites();

    eyes_init();

    bonus_init();

    projectile_init_all();
}

void clouds_init() BANKED
{
    HIDE_BKG;
    HIDE_SPRITES;
    HIDE_WIN;

    state_init();
    tc_init();
    text_init();

    trig_clouds.check = fn_clouds;
    trig_clouds.next = 0;
    trigger_init(&trig_clouds);

    // Init background tiles and snake body
    clouds_init_background();

    // Init sprites, eyes and bonus
    clouds_init_sprites();

    refresh_OAM();

    SHOW_BKG;
    SHOW_SPRITES;
}
