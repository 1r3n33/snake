#pragma bank 4
#include "spider.h"
#include "../camera.h"
#include "../projectile.h"

typedef struct Spider
{
    uint16_t origin_x;
    uint16_t origin_y;
    uint16_t x;
    uint16_t y;
} Spider;

#define SPIDER_MAX_COUNT 1U

#define SPIDER_SPRITE_0 32U

Spider spiders[SPIDER_MAX_COUNT];

void spider_init_all() BANKED
{
    for (uint8_t i = 0U; i < SPIDER_MAX_COUNT; i++)
    {
        spider_init(i);
    }
}

void spider_init(const uint8_t id) BANKED
{
    Spider *s = spiders + id;
    s->origin_x = 0U;
    s->origin_y = 0U;
    s->x = 0U;
    s->y = 0U;

    uint8_t sprite_id = SPIDER_SPRITE_0 + (id * 8U);

    set_sprite_tile(sprite_id, 48);
    set_sprite_prop(sprite_id, 0);
    move_sprite(sprite_id, 0U, 0U);
    sprite_id++;

    set_sprite_tile(sprite_id, 49);
    set_sprite_prop(sprite_id, 0);
    move_sprite(sprite_id, 0U, 0U);
    sprite_id++;

    set_sprite_tile(sprite_id, 50);
    set_sprite_prop(sprite_id, 0);
    move_sprite(sprite_id, 0U, 0U);
    sprite_id++;

    set_sprite_tile(sprite_id, 51);
    set_sprite_prop(sprite_id, 0);
    move_sprite(sprite_id, 0U, 0U);
    sprite_id++;

    set_sprite_tile(sprite_id, 50);
    set_sprite_prop(sprite_id, S_FLIPX);
    move_sprite(sprite_id, 0U, 0U);
    sprite_id++;

    set_sprite_tile(sprite_id, 51);
    set_sprite_prop(sprite_id, S_FLIPX);
    move_sprite(sprite_id, 0U, 0U);
    sprite_id++;

    set_sprite_tile(sprite_id, 48);
    set_sprite_prop(sprite_id, S_FLIPX);
    move_sprite(sprite_id, 0U, 0U);
    sprite_id++;

    set_sprite_tile(sprite_id, 49);
    set_sprite_prop(sprite_id, S_FLIPX);
    move_sprite(sprite_id, 0U, 0U);
}

void spider_update_all() BANKED
{
    for (uint8_t i = 0U; i < SPIDER_MAX_COUNT; i++)
    {
        spider_update(i);
    }
}

void spider_update(const uint8_t id) BANKED
{
    Spider *s = spiders + id;

    // Update

    uint16_t s_left = s->x;
    uint16_t s_right = s->x + 32U;
    uint16_t s_top = s->y;
    uint16_t s_bottom = s->y + 16U;

    // Check Collision with projectile
    if (projectile_check_collision(s_top, s_left, s_bottom, s_right))
    {
        s->origin_x = 0U;
        s->origin_y = 0U;
        s->x = 0U;
        s->y = 0U;
        uint8_t sprite_id = SPIDER_SPRITE_0 + (id * 8U);
        move_sprite(sprite_id++, 0U, 0U);
        move_sprite(sprite_id++, 0U, 0U);
        move_sprite(sprite_id++, 0U, 0U);
        move_sprite(sprite_id++, 0U, 0U);
        move_sprite(sprite_id++, 0U, 0U);
        move_sprite(sprite_id++, 0U, 0U);
        move_sprite(sprite_id++, 0U, 0U);
        move_sprite(sprite_id, 0U, 0U);
        return;
    }

    Camera *cam = camera_get();
    uint16_t cam_left = cam->sx;
    uint16_t cam_right = cam_left + DEVICE_SCREEN_PX_WIDTH;
    uint16_t cam_top = cam->sy;
    uint16_t cam_bottom = cam_top + DEVICE_SCREEN_PX_HEIGHT;

    if (s_left < cam_right && s_right > cam_left && s_top < cam_bottom && s_bottom > cam_top)
    {
        uint8_t x = DEVICE_SPRITE_PX_OFFSET_X + s_left - (cam->sx % 256U);
        uint8_t y = DEVICE_SPRITE_PX_OFFSET_Y + s_top - (cam->sy % 256U);

        uint8_t sprite_id = SPIDER_SPRITE_0 + (id * 8U);
        move_sprite(sprite_id++, x, y);
        move_sprite(sprite_id++, x, y + 8U);
        move_sprite(sprite_id++, x + 8U, y);
        move_sprite(sprite_id++, x + 8U, y + 8U);
        move_sprite(sprite_id++, x + 16U, y);
        move_sprite(sprite_id++, x + 16U, y + 8U);
        move_sprite(sprite_id++, x + 24U, y);
        move_sprite(sprite_id, x + 24U, y + 8U);
    }
    else
    {
        uint8_t sprite_id = SPIDER_SPRITE_0 + (id * 8U);
        move_sprite(sprite_id++, 0U, 0U);
        move_sprite(sprite_id++, 0U, 0U);
        move_sprite(sprite_id++, 0U, 0U);
        move_sprite(sprite_id++, 0U, 0U);
        move_sprite(sprite_id++, 0U, 0U);
        move_sprite(sprite_id++, 0U, 0U);
        move_sprite(sprite_id++, 0U, 0U);
        move_sprite(sprite_id, 0U, 0U);
    }
}

void spider_spawn(const uint16_t x, const uint16_t y) BANKED
{
    Spider *s = spiders + 0;
    s->origin_x = x;
    s->origin_y = y;
    s->x = x;
    s->y = y;
}
