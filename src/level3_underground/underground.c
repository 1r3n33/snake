#pragma bank 4
#include <gb/gb.h>
#include "spider.h"
#include "underground.h"
#include "../actor.h"
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

#include "../../res/level3_underground/underground_tilemap.h"
#include "../../res/level3_underground/underground_tileset.h"

#include "../../res/level3_underground/frog_intro.inc"

#define FROG_ACTOR_ID 0U
#define FROG_SPRITE_ID 12U

const uint8_t underground_snake_tile_offset[16] = {
    GFX_SNAKE_OFFSET_0, GFX_SNAKE_OFFSET_1, GFX_SNAKE_OFFSET_2, GFX_SNAKE_OFFSET_3,
    GFX_SNAKE_OFFSET_2, GFX_SNAKE_OFFSET_2, GFX_SNAKE_OFFSET_2, GFX_SNAKE_OFFSET_2,
    GFX_SNAKE_OFFSET_1, GFX_SNAKE_OFFSET_1, GFX_SNAKE_OFFSET_1, GFX_SNAKE_OFFSET_1,
    GFX_SNAKE_OFFSET_1, GFX_SNAKE_OFFSET_1, GFX_SNAKE_OFFSET_1, GFX_SNAKE_OFFSET_1};

Trigger trig_underground;

uint8_t text_shown = 0;

uint8_t fn_underground() BANKED
{
    actor_update(FROG_ACTOR_ID);
    spider_update_all();

    State *state = state_get();
    if (state->ko == 1)
    {
        return TRIGGER_RESTART_LEVEL;
    }

    SnakeNode *head = snake_get_head();
    if (head->x < 48 && !text_shown)
    {
        text_show(text_frog_intro);
        text_shown = 1;
        actor_disable(FROG_ACTOR_ID);
        return TRIGGER_CONTINUE;
    }

    if (head->x == 62U && head->y == 2U)
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

    const uint8_t dirs[] = {DIRECTION_SOUTH, DIRECTION_SOUTH, DIRECTION_WEST, DIRECTION_WEST, DIRECTION_WEST};
    snake_init(56, 50, dirs, sizeof(dirs));

    // Init camera right after the head has been set AND before initializing sprites.
    // Sprites require camera location to be placed at the desired position.
    camera_init();
}

const int8_t frog_xy_offsets[32] = {
    0, 0, 8, 0, 16, 0, 24, 0,
    0, 8, 8, 8, 16, 8, 24, 8,
    0, 16, 8, 16, 16, 16, 24, 16,
    0, 24, 8, 24, 16, 24, 24, 24};

const uint8_t frog_tile_ids[16] = {
    80U, 82U, 82U, 80U,
    81U, 83U, 83U, 81U,
    84U, 86U, 86U, 84U,
    85U, 87U, 87U, 85U};

const uint8_t frog_tile_props[16] = {
    0U, 0U, S_FLIPX, S_FLIPX,
    0U, 0U, S_FLIPX, S_FLIPX,
    0U, 0U, S_FLIPX, S_FLIPX,
    0U, 0U, S_FLIPX, S_FLIPX};

void underground_init_sprites() BANKED
{
    gfx_load_sprites();

    eyes_init();

    bonus_init();

    projectile_init_all();

    spider_init_all();
    spider_spawn(30U, 80U);

    actor_reset(FROG_ACTOR_ID);
    actor_set_sprite_range(FROG_ACTOR_ID, FROG_SPRITE_ID, 16U);
    actor_set_xy(FROG_ACTOR_ID, 324U, 436U);
    actor_set_xy_offsets(FROG_ACTOR_ID, frog_xy_offsets);
    actor_set_bounding_box(FROG_ACTOR_ID, 0, 32, 0, 32);
    actor_set_tile_ids(FROG_ACTOR_ID, frog_tile_ids);
    actor_set_tile_props(FROG_ACTOR_ID, frog_tile_props);
    actor_enable(FROG_ACTOR_ID);
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
