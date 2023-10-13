#pragma bank 2

#include <gb/gb.h>

#include "garden.h"

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

#include "../../res/level1_garden/gfx_background.h"
#include "../../res/level1_garden/gfx_garden.h"
#include "../../res/level1_garden/gfx_sprites.h"

#include "../../res/level1_garden/mole_intro.inc"
#include "../../res/level1_garden/mole_pass.inc"

#define MOLE_ACTOR_ID 0U
#define MOLE_SPRITE_ID 12U

const uint8_t garden_snake_tile_offset[128] = {
    GFX_SNAKE_OFFSET_0, GFX_SNAKE_OFFSET_0, GFX_SNAKE_OFFSET_0, GFX_SNAKE_OFFSET_0, GFX_SNAKE_OFFSET_0, GFX_SNAKE_OFFSET_0, GFX_SNAKE_OFFSET_0, GFX_SNAKE_OFFSET_0,
    GFX_SNAKE_OFFSET_0, GFX_SNAKE_OFFSET_0, GFX_SNAKE_OFFSET_3, GFX_SNAKE_OFFSET_3, GFX_SNAKE_OFFSET_0, GFX_SNAKE_OFFSET_0, GFX_SNAKE_OFFSET_3, GFX_SNAKE_OFFSET_3,
    GFX_SNAKE_OFFSET_0, GFX_SNAKE_OFFSET_0, GFX_SNAKE_OFFSET_0, GFX_SNAKE_OFFSET_0, GFX_SNAKE_OFFSET_3, GFX_SNAKE_OFFSET_3, GFX_SNAKE_OFFSET_3, GFX_SNAKE_OFFSET_3};

// Meet with the mole, to activate apples collection.
Trigger trig_mole_visit_1;
Trigger trig_mole_begin_dialog_1;
Trigger trig_mole_end_dialog_1;
Trigger trig_collect_apples;
Trigger trig_mole_visit_2;
Trigger trig_mole_begin_dialog_2;
Trigger trig_mole_end_dialog_2;
Trigger trig_mole_enter_hole;

uint8_t fn_mole_visit() BANKED
{
    actor_update(MOLE_ACTOR_ID);

    State *state = state_get();
    if (state->ko == 1)
    {
        return TRIGGER_RESTART_LEVEL;
    }

    SnakeNode *head = snake_get_head();
    if (head->x > 53 && head->y > 50)
    {
        const uint16_t tx = ((uint16_t)BACKGROUND_WIDTH * 8U) - (DEVICE_SCREEN_PX_WIDTH / 2U);
        const uint16_t ty = ((uint16_t)BACKGROUND_HEIGHT * 8U) - (DEVICE_SCREEN_PX_HEIGHT / 2U);
        snake_enable_update(0);
        camera_set_xy_target(tx, ty);
        return TRIGGER_NEXT_TRIGGER;
    }
    return TRIGGER_CONTINUE;
}

uint8_t fn_mole_begin_dialog(const char *text) BANKED
{
    actor_update(MOLE_ACTOR_ID);

    Camera *cam = camera_get();
    if (cam->cx == cam->tx && cam->cy == cam->ty)
    {
        text_show(text);
        actor_disable(MOLE_ACTOR_ID);

        SnakeNode *head = snake_get_head();
        camera_set_snake_target(head);

        return TRIGGER_NEXT_TRIGGER;
    }

    return TRIGGER_CONTINUE;
}

uint8_t fn_mole_begin_dialog_1() BANKED
{
    return fn_mole_begin_dialog(text_mole_intro);
}

uint8_t fn_mole_begin_dialog_2() BANKED
{
    uint8_t res = fn_mole_begin_dialog(text_mole_pass);

    // When dialog is over, remove the grid from the hole.
    if (res == TRIGGER_NEXT_TRIGGER)
    {
        uint16_t bkg_offset = (uint16_t)((54 * BACKGROUND_WIDTH) + 54);
        background_update(bkg_offset + 0, 11);
        background_update(bkg_offset + 1, 12);
        background_update(bkg_offset + BACKGROUND_WIDTH + 0, 15);
        background_update(bkg_offset + BACKGROUND_WIDTH + 1, 16);

        uint16_t vram_offset = (uint16_t)(((54 % DEVICE_SCREEN_BUFFER_HEIGHT) * DEVICE_SCREEN_BUFFER_WIDTH) + (54 % DEVICE_SCREEN_BUFFER_WIDTH));

        // Wait for VBLANK to get access to the VRAM.
        while ((STAT_REG & 3) != 1)
            ;

        *((uint8_t *)0x9800U + vram_offset + 0) = 11;
        *((uint8_t *)0x9800U + vram_offset + 1) = 12;
        *((uint8_t *)0x9800U + vram_offset + DEVICE_SCREEN_BUFFER_WIDTH + 0) = 15;
        *((uint8_t *)0x9800U + vram_offset + DEVICE_SCREEN_BUFFER_WIDTH + 1) = 16;

        // Wait for VBLANK to end.
        while ((STAT_REG & 3) == 1)
            ;
    }

    return res;
}

uint8_t fn_mole_end_dialog() BANKED
{
    Camera *cam = camera_get();
    if (cam->cx == cam->tx && cam->cy == cam->ty)
    {
        snake_enable_update(1);
        bonus_set_spawn_zone_rect(4, 32, 4, 50);
        return TRIGGER_NEXT_TRIGGER;
    }

    return TRIGGER_CONTINUE;
}

uint8_t fn_collect_apples() BANKED
{
    State *state = state_get();
    if (state->ko == 1)
    {
        return TRIGGER_RESTART_LEVEL;
    }

    if (state->score == 5)
    {
        actor_enable(MOLE_ACTOR_ID);
        return TRIGGER_NEXT_TRIGGER;
    }

    return TRIGGER_CONTINUE;
}

uint8_t fn_enter_hole() BANKED
{
    State *state = state_get();
    if (state->ko == 1)
    {
        return TRIGGER_RESTART_LEVEL;
    }

    SnakeNode *head = snake_get_head();
    if (head->x == 54 && head->y == 54)
    {
        eyes_hide();
        snake_lock_head(255U);

        return snake_length() > 0 ? TRIGGER_CONTINUE : TRIGGER_NEXT_TRIGGER;
    }

    return TRIGGER_CONTINUE;
}

void garden_init_background() BANKED
{
    // Load Background tiles and then map
    set_bkg_data(0, gfx_backgroundLen, gfx_background);

    background_init(gfx_garden, garden_snake_tile_offset);

    const uint8_t dirs[] = {DIRECTION_EAST, DIRECTION_EAST, DIRECTION_EAST};
    snake_init(4, 8, dirs, sizeof(dirs));

    // Init camera right after the head has been set AND before initializing sprites.
    // Sprites require camera location to be placed at the desired position.
    camera_init();
}

const int8_t mole_xy_offsets[32] = {
    0, 0, 8, 0, 16, 0, 24, 0,
    0, 8, 8, 8, 16, 8, 24, 8,
    0, 16, 8, 16, 16, 16, 24, 16,
    0, 24, 8, 24, 16, 24, 24, 24};

const uint8_t mole_tile_ids[16] = {
    0U, 65U, 71U, 0U,
    64U, 66U, 72U, 73U,
    67U, 69U, 74U, 75U,
    68U, 70U, 77U, 76U};

const uint8_t mole_tile_props[16] = {
    0U, 0U, 0U, 0U,
    0U, 0U, 0U, 0U,
    0U, 0U, 0U, 0U,
    0U, 0U, 0U, 0U};

void garden_init_sprites() BANKED
{
    set_sprite_data(0, gfx_spritesLen, gfx_sprites);

    eyes_init();

    bonus_init();

    projectile_init_all();

    actor_reset(MOLE_ACTOR_ID);
    actor_set_sprite_range(MOLE_ACTOR_ID, MOLE_SPRITE_ID, 16U);
    actor_set_xy(MOLE_ACTOR_ID, 53U * 8U, 52U * 8U);
    actor_set_xy_offsets(MOLE_ACTOR_ID, mole_xy_offsets);
    actor_set_bounding_box(MOLE_ACTOR_ID, 0, 32, 0, 32);
    actor_set_tile_ids(MOLE_ACTOR_ID, mole_tile_ids);
    actor_set_tile_props(MOLE_ACTOR_ID, mole_tile_props);
    actor_enable(MOLE_ACTOR_ID);
}

void garden_init() BANKED
{
    HIDE_BKG;
    HIDE_SPRITES;
    HIDE_WIN;

    state_init();
    tc_init();
    text_init();

    trig_mole_visit_1.check = fn_mole_visit;
    trig_mole_visit_1.next = &trig_mole_begin_dialog_1;
    trig_mole_begin_dialog_1.check = fn_mole_begin_dialog_1;
    trig_mole_begin_dialog_1.next = &trig_mole_end_dialog_1;
    trig_mole_end_dialog_1.check = fn_mole_end_dialog;
    trig_mole_end_dialog_1.next = &trig_collect_apples;
    trig_collect_apples.check = fn_collect_apples;
    trig_collect_apples.next = &trig_mole_visit_2;
    trig_mole_visit_2.check = fn_mole_visit;
    trig_mole_visit_2.next = &trig_mole_begin_dialog_2;
    trig_mole_begin_dialog_2.check = fn_mole_begin_dialog_2;
    trig_mole_begin_dialog_2.next = &trig_mole_end_dialog_2;
    trig_mole_end_dialog_2.check = fn_mole_end_dialog;
    trig_mole_end_dialog_2.next = &trig_mole_enter_hole;
    trig_mole_enter_hole.check = fn_enter_hole;
    trig_mole_enter_hole.next = 0;

    trigger_init(&trig_mole_visit_1);

    // Init background tiles, snake body and camera
    garden_init_background();

    // Init sprites, eyes and bonus
    garden_init_sprites();

    // No bonus spawn at the beginning
    bonus_set_spawn_zone_rect(0, 0, 0, 0);

    refresh_OAM();

    SHOW_BKG;
    SHOW_SPRITES;
}
