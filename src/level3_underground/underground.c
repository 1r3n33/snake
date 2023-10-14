#pragma bank 4
#include <gb/gb.h>
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

#define SPIDER_ACTOR_ID 1U
#define SPIDER_SPRITE_ID 32U

const uint8_t underground_snake_tile_offset[16] = {
    GFX_SNAKE_OFFSET_0, GFX_SNAKE_OFFSET_1, GFX_SNAKE_OFFSET_2, GFX_SNAKE_OFFSET_3,
    GFX_SNAKE_OFFSET_2, GFX_SNAKE_OFFSET_2, GFX_SNAKE_OFFSET_2, GFX_SNAKE_OFFSET_2,
    GFX_SNAKE_OFFSET_1, GFX_SNAKE_OFFSET_1, GFX_SNAKE_OFFSET_1, GFX_SNAKE_OFFSET_1,
    GFX_SNAKE_OFFSET_1, GFX_SNAKE_OFFSET_1, GFX_SNAKE_OFFSET_1, GFX_SNAKE_OFFSET_1};

Trigger trig_underground;

uint8_t text_shown = 0;

typedef struct ActorController
{
    uint8_t first;   // id of the first actor (actors are in sequence)
    uint8_t count;   // number of actors
    uint16_t *zones; // zones (1 zone per actor, lrtb)
    uint8_t active;  // active actor id else 0xFF
} ActorController;

ActorController spiders;

const uint16_t spiders_zones[] = {
    31U * 8U, 35U * 8U, 54U * 8U, 56U * 8U,
    7U * 8U, 11U * 8U, 32U * 8U, 34U * 8U};

const uint16_t spiders_xy[] = {
    31U * 8U,
    54U * 8U,
    7U * 8U,
    32U * 8U,
};

const int8_t spider_xy_offsets[] = {
    0, 0, 8, 0, 16, 0, 24, 0,
    0, 8, 8, 8, 16, 8, 24, 8};

const uint8_t spider_tile_ids[] = {
    48U, 50U, 50U, 48U,
    49U, 51U, 51U, 49U};

const uint8_t spider_tile_props[] = {
    0U, 0U, S_FLIPX, S_FLIPX,
    0U, 0U, S_FLIPX, S_FLIPX};

void spiders_init() BANKED
{
    spiders.first = SPIDER_ACTOR_ID;
    spiders.count = 2U;
    spiders.zones = (uint16_t *)spiders_zones;

    for (uint8_t i = 0U; i < spiders.count; i++)
    {
        actor_reset(SPIDER_ACTOR_ID + i);
        actor_set_sprite_range(SPIDER_ACTOR_ID + i, SPIDER_SPRITE_ID, 8U);
        actor_set_xy(SPIDER_ACTOR_ID + i, spiders_xy[i * 2], spiders_xy[i * 2 + 1]);
        actor_set_xy_offsets(SPIDER_ACTOR_ID + i, spider_xy_offsets);
        actor_set_bounding_box(SPIDER_ACTOR_ID + i, 0, 32, 0, 16);
        actor_set_tile_ids(SPIDER_ACTOR_ID + i, spider_tile_ids);
        actor_set_tile_props(SPIDER_ACTOR_ID + i, spider_tile_props);
        actor_enable(SPIDER_ACTOR_ID + i);
    }
}

// Update first visible spider.
void spiders_update() BANKED
{
    Camera *cam = camera_get();
    uint16_t cam_left = cam->sx;
    uint16_t cam_right = cam_left + DEVICE_SCREEN_PX_WIDTH;
    uint16_t cam_top = cam->sy;
    uint16_t cam_bottom = cam_top + DEVICE_SCREEN_PX_HEIGHT;

    // Find the active actor id
    uint8_t active = 0xFFU;
    uint16_t *p = spiders.zones;
    for (uint8_t i = 0U; i < spiders.count; i++)
    {
        uint16_t zone_left = *p++;
        uint16_t zone_right = *p++;
        uint16_t zone_top = *p++;
        uint16_t zone_bottom = *p++;
        if (zone_left < cam_right && zone_right > cam_left && zone_top < cam_bottom && zone_bottom > cam_top)
        {
            active = spiders.first + i;
            break;
        }
    }

    // Swap active actor (hide previous).
    if (active != spiders.active)
    {
        if (spiders.active != 0xFFU)
        {
            actor_hide(spiders.active);
        }
        spiders.active = active;
    }

    // Update active actor
    if (spiders.active != 0xFF)
    {
        actor_update(spiders.active);
    }
}

uint8_t fn_underground() BANKED
{
    actor_update(FROG_ACTOR_ID);
    spiders_update();

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

    actor_reset(FROG_ACTOR_ID);
    actor_set_sprite_range(FROG_ACTOR_ID, FROG_SPRITE_ID, 16U);
    actor_set_xy(FROG_ACTOR_ID, 324U, 436U);
    actor_set_xy_offsets(FROG_ACTOR_ID, frog_xy_offsets);
    actor_set_bounding_box(FROG_ACTOR_ID, 0, 32, 0, 32);
    actor_set_tile_ids(FROG_ACTOR_ID, frog_tile_ids);
    actor_set_tile_props(FROG_ACTOR_ID, frog_tile_props);
    actor_enable(FROG_ACTOR_ID);

    spiders_init();
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
