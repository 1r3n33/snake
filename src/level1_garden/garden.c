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
#include "../trigger.h"

#include "../../res/level1_garden/gfx_background.h"
#include "../../res/level1_garden/gfx_garden.h"
#include "../../res/level1_garden/gfx_sprites.h"

const uint8_t garden_snake_tile_offset[128] = {0};

const uint8_t text_mole_intro[] = {
    // Hello! My name is
    90, 87, 94, 94, 97, 109, 82, 95, 107, 82, 96, 83, 95, 87, 82, 91, 101, 0,
    // Molly...
    95, 97, 94, 94, 107, 110, 110, 110, 0,
    // It's lunch time
    91, 102, 111, 101, 82, 94, 103, 96, 85, 90, 82, 102, 91, 95, 87, 0,
    // and I'm hungry!
    83, 96, 86, 82, 91, 111, 95, 82, 90, 103, 96, 89, 100, 107, 109, 0,
    // Please bring 5
    98, 94, 87, 83, 101, 87, 82, 84, 100, 91, 96, 89, 82, 77, 0,
    // apples to me.
    83, 98, 98, 94, 87, 101, 82, 102, 97, 82, 95, 87, 110, 0,
    // Thank you!
    102, 90, 83, 96, 93, 82, 107, 97, 103, 109, 0,
    // EOF
    0};

// Meet with the mole, to activate apples collection.
Trigger trig_mole_visit_1;
Trigger trig_mole_begin_dialog_1;
Trigger trig_mole_end_dialog_1;
Trigger trig_collect_apples;

uint8_t fn_mole_visit_1() BANKED
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

uint8_t fn_mole_begin_dialog_1() BANKED
{
    mole_update();

    Camera *cam = camera_get();
    if (cam->cx == cam->tx && cam->cy == cam->ty)
    {
        text_show(text_mole_intro);
        mole_hide();

        SnakeNode *head = snake_get_head();
        camera_set_snake_target(head);

        return TRIGGER_NEXT_TRIGGER;
    }

    return TRIGGER_CONTINUE;
}

uint8_t fn_mole_end_dialog_1() BANKED
{
    Camera *cam = camera_get();
    if (cam->cx == cam->tx && cam->cy == cam->ty)
    {
        snake_enable_update(1);
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
        return TRIGGER_NEXT_TRIGGER;
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

    trig_mole_visit_1.check = fn_mole_visit_1;
    trig_mole_visit_1.next = &trig_mole_begin_dialog_1;
    trig_mole_begin_dialog_1.check = fn_mole_begin_dialog_1;
    trig_mole_begin_dialog_1.next = &trig_mole_end_dialog_1;
    trig_mole_end_dialog_1.check = fn_mole_end_dialog_1;
    trig_mole_end_dialog_1.next = &trig_collect_apples;
    trig_collect_apples.check = fn_collect_apples;
    trig_collect_apples.next = 0;
    trigger_init(&trig_mole_visit_1);

    // Init background tiles, snake body and camera
    garden_init_background();

    // Init sprites, eyes and bonus
    garden_init_sprites();

    refresh_OAM();

    SHOW_BKG;
    SHOW_SPRITES;
}
