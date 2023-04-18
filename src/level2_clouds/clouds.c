#pragma bank 3
#include <gb/gb.h>
#include "clouds.h"
#include "../background.h"
#include "../bonus.h"
#include "../camera.h"
#include "../direction.h"
#include "../eyes.h"
#include "../game.h"
#include "../snake.h"
#include "../state.h"
#include "../tiles_update.h"

#include "../../res/level2_clouds/clouds_tilemap.h"
#include "../../res/level2_clouds/clouds_tileset.h"

void clouds_init_background() BANKED
{
// TODO: Remove duplicates
#define FRAME_COUNT 2
    const uint8_t _snake_tiles_tail_E[FRAME_COUNT * 4] = {192, 178, 193, 179, 124, 192, 124, 193};
    const uint8_t _snake_tiles_body_H[FRAME_COUNT * 4] = {178, 178, 179, 179, 178, 178, 179, 179};
    const uint8_t _snake_tiles_head_E[FRAME_COUNT * 4] = {194, 124, 195, 124, 178, 194, 179, 195};

    // Load Background tiles and then map
    set_bkg_data(0, clouds_tilesetLen, clouds_tileset);

    background_init(clouds_tilemap);
    set_bkg_submap(0U, 0U, DEVICE_SCREEN_WIDTH + 1, DEVICE_SCREEN_HEIGHT + 1, background_get(), BACKGROUND_WIDTH);

    // Setup initial snake
    snake_init();

    int x_pos = 2;

    SnakeNode *node = snake_get_head();
    node->x = x_pos;
    node->y = 2;
    node->offset_x = 0;
    node->offset_y = 8;
    node->in = DIRECTION_UNKNOWN;
    node->out = DIRECTION_EAST;
    node->tiles = _snake_tiles_tail_E;
    tu_apply(node);

    for (uint8_t i = 0; i < 2; i++)
    {
        node = snake_advance_head();
        x_pos += 2;

        node->x = x_pos;
        node->y = 2;
        node->offset_x = 0;
        node->offset_y = 8;
        node->in = DIRECTION_EAST;
        node->out = DIRECTION_EAST;
        node->tiles = _snake_tiles_body_H;
        tu_apply(node);
    }

    node = snake_advance_head();
    x_pos += 2;

    node->x = x_pos;
    node->y = 2;
    node->offset_x = 0;
    node->offset_y = 8;
    node->in = DIRECTION_EAST;
    node->out = DIRECTION_UNKNOWN;
    node->tiles = _snake_tiles_head_E;
    tu_apply_with_direction(node, DIRECTION_EAST);
}

void clouds_init_sprites() BANKED
{
    // set_sprite_data(0, gfx_spritesLen, gfx_sprites);

    eyes_init();
    eyes_move(snake_get_head());

    bonus_init();
}

void clouds_init() BANKED
{
    HIDE_BKG;
    HIDE_SPRITES;

    // Init background tiles and snake body
    clouds_init_background();

    // Init sprites, eyes and bonus
    clouds_init_sprites();

    camera_init(snake_get_head());

    SHOW_BKG;
    SHOW_SPRITES;
}

int8_t clouds_loop_check() BANKED
{
    State *state = state_get();
    if (state->score == 2)
    {
        return 1;
    }

    return 0;
}

int8_t clouds_loop() BANKED
{
    return game_loop(clouds_loop_check);
}
