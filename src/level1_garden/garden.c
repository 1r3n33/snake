#pragma bank 2

#include <gb/gb.h>

#include "garden.h"
#include "mole.h"

#include "../background.h"
#include "../bonus.h"
#include "../camera.h"
#include "../eyes.h"
#include "../game.h"
#include "../projectile.h"
#include "../snake.h"
#include "../state.h"
#include "../text.h"
#include "../tiles_copy.h"
#include "../trigger.h"

#include "../../res/level1_garden/gfx_background.h"
#include "../../res/level1_garden/gfx_garden.h"
#include "../../res/level1_garden/gfx_sprites.h"

const uint8_t garden_snake_tile_offset[128] = {
    0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x00, 0x00, 0x30, 0x30, 0x00, 0x00, 0x30, 0x30,
    0x00, 0x00, 0x00, 0x00, 0x30, 0x30, 0x30, 0x30};

const uint8_t text_mole_intro[] = {
    // Hello! My name is
    90, 87, 94, 94, 97, 109, 82, 95, 107, 82, 96, 83, 95, 87, 82, 91, 101, 0,
    // Molly...
    95, 97, 94, 94, 107, 110, 110, 110, 0,
    // If you bring me
    91, 88, 82, 107, 97, 103, 82, 84, 100, 91, 96, 89, 82, 95, 87, 0,
    // 5 apples...
    77, 82, 83, 98, 98, 94, 87, 101, 110, 110, 110, 0,
    // I'll show you
    91, 111, 94, 94, 82, 101, 90, 97, 105, 82, 107, 97, 103, 0,
    // the exit!
    102, 90, 87, 82, 87, 106, 91, 102, 109, 0,
    // Good luck!
    89, 97, 97, 86, 82, 94, 103, 85, 93, 109, 0,
    // EOF
    0};

const uint8_t text_mole_pass[] = {
    // Hey! Thank you
    90, 87, 107, 109, 82, 102, 90, 83, 96, 93, 82, 107, 97, 103, 0,
    // so much...
    101, 97, 82, 95, 103, 85, 90, 110, 110, 110, 0,
    // Follow me
    88, 97, 94, 94, 97, 105, 82, 95, 87, 0,
    // underground!
    103, 96, 86, 87, 100, 89, 100, 97, 103, 96, 86, 109, 0,
    // EOF
    0};

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
    mole_update();

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
    mole_update();

    Camera *cam = camera_get();
    if (cam->cx == cam->tx && cam->cy == cam->ty)
    {
        text_show(text);
        mole_hide();

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
        mole_show();
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

    bonus_init();

    projectile_init_all();

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
