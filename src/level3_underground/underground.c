#pragma bank 4
#include <gb/gb.h>
#include <string.h>
#include "underground.h"
#include "../background.h"
#include "../bonus.h"
#include "../camera.h"
#include "../direction.h"
#include "../eyes.h"
#include "../game.h"
#include "../graphics.h"
#include "../snake.h"
#include "../state.h"
#include "../text.h"
#include "../tiles_copy.h"
#include "../trigger.h"

#include "../ecs/collision_system.h"
#include "../ecs/graphic_system.h"
#include "../ecs/projectile_component.h"
#include "../ecs/projectile_system.h"
#include "../ecs/spider_component.h"
#include "../ecs/spider_system.h"

#include "../../res/level3_underground/underground_tilemap.h"
#include "../../res/level3_underground/underground_tileset.h"

#include "../../res/level3_underground/frog_intro.inc"

#define FROG_ACTOR_ID 0U
#define FROG_SPRITE_ID 12U

#define SPIDER_ACTOR_ID 1U
#define SPIDER_SPRITE_ID 32U

#define PROJECTILE_ACTOR_ID 5U
#define PROJECTILE_SPRITE_ID 28U

#define PROJECTILE_COUNT 3U

#define FROG_GFX_COUNT 1U
#define SPIDER_GFX_COUNT 1U
#define PROJECTILE_GFX_COUNT PROJECTILE_COUNT

GraphicComponent *frog_gfx = gfx_components + 0U;
GraphicComponent *spider_gfx = gfx_components + FROG_GFX_COUNT;
GraphicComponent *projectiles_gfx = gfx_components + FROG_GFX_COUNT + SPIDER_GFX_COUNT;

#define SPIDER_COL_COUNT 1U
#define PROJECTILE_COL_COUNT PROJECTILE_COUNT
CollisionComponent *spider_col = col_collidees + 0U;
CollisionComponent *projectiles_col = col_colliders + 0U;

const uint8_t underground_snake_tile_offset[16] = {
    GFX_SNAKE_OFFSET_0, GFX_SNAKE_OFFSET_1, GFX_SNAKE_OFFSET_2, GFX_SNAKE_OFFSET_3,
    GFX_SNAKE_OFFSET_2, GFX_SNAKE_OFFSET_2, GFX_SNAKE_OFFSET_2, GFX_SNAKE_OFFSET_2,
    GFX_SNAKE_OFFSET_1, GFX_SNAKE_OFFSET_1, GFX_SNAKE_OFFSET_1, GFX_SNAKE_OFFSET_1,
    GFX_SNAKE_OFFSET_1, GFX_SNAKE_OFFSET_1, GFX_SNAKE_OFFSET_1, GFX_SNAKE_OFFSET_1};

Trigger trig_underground;

uint8_t text_shown = 0;

uint8_t fn_underground() BANKED
{
    // Experiencing a compiler issue with function parameter when making function pointer call.
    // Call joypad() instead of relying on a 'pressed' function parameter.
    uint8_t pressed = joypad();

    proj_sys_process(pressed);
    spd_sys_process();
    col_sys_process();
    gfx_sys_process();

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
        gfx_disable(frog_gfx);
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

const int8_t frog_yx_offsets[32] = {
    0, 0, 0, 8, 0, 16, 0, 24,
    8, 0, 8, 8, 8, 16, 8, 24,
    16, 0, 16, 8, 16, 16, 16, 24,
    24, 0, 24, 8, 24, 16, 24, 24};

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

const int8_t spider_yx_offsets[] = {
    0, 0, 0, 8, 0, 16, 0, 24,
    8, 0, 8, 8, 8, 16, 8, 24};

const uint8_t spider_tile_ids[] = {
    48U, 50U, 50U, 48U,
    49U, 51U, 51U, 49U};

const uint8_t spider_tile_props[] = {
    0U, 0U, S_FLIPX, S_FLIPX,
    0U, 0U, S_FLIPX, S_FLIPX};

const uint8_t projectile_yx_offsets[] = {
    0U, 0U};

void on_proj_col() BANKED
{
    uint8_t id = col_sys_get_data();
    ProjectileComponent *proj = proj_components + id;
    proj->enabled = 0U;

    col_sys_bkg_collider_disable(id);
    col_sys_c2c_collider_disable(id);

    GraphicComponent *gfx = projectiles_gfx + id;
    gfx_disable(gfx);
}

void on_spd_col() BANKED
{
    uint8_t id = col_sys_get_data();
    SpiderComponent *spd = spd_components + id;
    spd->enabled = 0U;

    gfx_disable(spider_gfx);
    col_sys_c2c_collidee_disable(0); // spider col is id 0
}

void underground_init_sprites() BANKED
{
    gfx_load_sprites();

    eyes_init();

    bonus_init();

    proj_sys_init(projectiles_gfx, projectiles_col);
    spd_sys_init(spider_gfx, spider_col);
    gfx_sys_init();
    col_sys_init();

    // Frog
    gfx_set_sprite_range(frog_gfx, FROG_SPRITE_ID, 16U);
    gfx_set_xy(frog_gfx, 324U, 436U);
    gfx_set_yx_offsets(frog_gfx, (uint8_t *)frog_yx_offsets);
    gfx_set_bounding_box(frog_gfx, 0, 32, 0, 32);
    gfx_set_tile_ids(frog_gfx, frog_tile_ids);
    gfx_set_tile_props(frog_gfx, frog_tile_props);
    gfx_enable(frog_gfx);

    // Spiders
    spd_components[0].enabled = 1U;
    spd_components[0].zone.l = 31U;
    spd_components[0].zone.r = 35U;
    spd_components[0].zone.t = 54U;
    spd_components[0].zone.b = 56U;

    spd_components[1].enabled = 1U;
    spd_components[1].zone.l = 7U;
    spd_components[1].zone.r = 11U;
    spd_components[1].zone.t = 32U;
    spd_components[1].zone.b = 34U;

    spider_col->on_c2c_col_cb = on_spd_col;

    gfx_set_sprite_range(spider_gfx, SPIDER_SPRITE_ID, 8U);
    gfx_set_yx_offsets(spider_gfx, (uint8_t *)spider_yx_offsets);
    gfx_set_bounding_box(spider_gfx, 0, 32, 0, 16);
    gfx_set_tile_ids(spider_gfx, spider_tile_ids);
    gfx_set_tile_props(spider_gfx, spider_tile_props);
    gfx_enable(spider_gfx);

    // Projectiles
    for (uint8_t i = 0U; i < PROJECTILE_COUNT; i++)
    {
        ProjectileComponent *proj = proj_components + i;
        proj->enabled = 0U;

        GraphicComponent *gfx = projectiles_gfx + i;
        gfx_set_sprite_range(gfx, PROJECTILE_SPRITE_ID + i, 1U);
        gfx_set_yx_offsets(gfx, (uint8_t *)projectile_yx_offsets);
        gfx_set_bounding_box(gfx, 0, 8, 0, 8);
        gfx_disable(gfx);

        CollisionComponent *col = projectiles_col + i;
        col->data = i;
        col->on_bkg_col_cb = on_proj_col;
        col->on_c2c_col_cb = on_proj_col;
        col_sys_c2c_collider_candidates(i, 0x1U); // pair proj col with spider col
    }

    // Process gfx once to position components properly.
    gfx_sys_process();
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
