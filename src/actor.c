#include <gb/gb.h>
#include "actor.h"
#include "camera.h"

#define ACTOR_MAX_COUNT 8U
#define ACTOR_MAX_TILE_COUNT 16U

#define ACTOR_STATE_DISABLED 0U
#define ACTOR_STATE_ENABLED 1U

typedef struct Actor
{
    uint16_t x;         // px in world coord (0 - 511)
    uint16_t y;         // px in world coord (0 - 511)
    uint8_t state;      //
    uint8_t timer;      //
    uint8_t id;         // sprite id (0 - 39)
    uint8_t count;      // sprite count (0 - ACTOR_MAX_TILE_COUNT-1)
    int8_t *xy_offsets; // xyxyxy...
    int8_t bb[4];       // bounding box (lrtb)
} Actor;

Actor actors[ACTOR_MAX_COUNT];

void actor_hide(uint8_t id)
{
    Actor *a = actors + id;

    uint8_t i = a->id;
    uint8_t c = a->count;
    while (c--)
    {
        move_sprite(i++, 0U, 0U);
    }
}

// Set all sprites position
void actor_move(uint8_t id)
{
    Actor *a = actors + id;

    uint16_t actor_left = a->x + a->bb[0U];
    uint16_t actor_right = a->x + a->bb[1U];
    uint16_t actor_top = a->y + a->bb[2U];
    uint16_t actor_bottom = a->y + a->bb[3U];

    Camera *cam = camera_get();
    uint16_t cam_left = cam->sx;
    uint16_t cam_right = cam_left + DEVICE_SCREEN_PX_WIDTH;
    uint16_t cam_top = cam->sy;
    uint16_t cam_bottom = cam_top + DEVICE_SCREEN_PX_HEIGHT;

    if (actor_left < cam_right && actor_right > cam_left && actor_top < cam_bottom && actor_bottom > cam_top)
    {
        uint8_t x = DEVICE_SPRITE_PX_OFFSET_X + actor_left - (cam->sx % 256U);
        uint8_t y = DEVICE_SPRITE_PX_OFFSET_Y + actor_top - (cam->sy % 256U);

        uint8_t i = a->id;
        uint8_t c = a->count;
        int8_t *p = a->xy_offsets;
        while (c--)
        {
            uint8_t tx = x + *p++;
            uint8_t ty = y + *p++;
            move_sprite(i++, tx, ty);
        }
    }
    else
    {
        actor_hide(id);
    }
}

void actor_reset(uint8_t id)
{
    Actor *a = actors + id;
    a->x = 0U;
    a->y = 0U;
    a->state = ACTOR_STATE_DISABLED;
    a->timer = 0U;
    a->id = 0U;
    a->count = 0U;
    a->xy_offsets = 0U;
    a->bb[0U] = 0;
    a->bb[1U] = 0;
    a->bb[2U] = 0;
    a->bb[3U] = 0;
}

void actor_set_sprite_range(uint8_t id, uint8_t first, uint8_t count)
{
    Actor *a = actors + id;
    a->id = first;
    a->count = count;
}

void actor_set_xy(uint8_t id, uint16_t x, uint16_t y)
{
    Actor *a = actors + id;
    a->x = x;
    a->y = y;
}

void actor_set_xy_offsets(uint8_t id, int8_t *offsets)
{
    Actor *a = actors + id;
    a->xy_offsets = offsets;
}

void actor_set_bounding_box(uint8_t id, int8_t left, int8_t right, int8_t top, int8_t bottom)
{
    Actor *a = actors + id;
    a->bb[0U] = left;
    a->bb[1U] = right;
    a->bb[2U] = top;
    a->bb[3U] = bottom;
}

void actor_set_tile_ids(uint8_t id, uint8_t *ids)
{
    Actor *a = actors + id;

    uint8_t i = a->id;
    uint8_t c = a->count;
    while (c--)
    {
        set_sprite_tile(i++, *ids++);
    }
}

void actor_set_tile_props(uint8_t id, uint8_t *props)
{
    Actor *a = actors + id;

    uint8_t i = a->id;
    uint8_t c = a->count;
    while (c--)
    {
        set_sprite_prop(i++, *props++);
    }
}

void actor_update(uint8_t id)
{
    Actor *a = actors + id;

    if (a->state == ACTOR_STATE_DISABLED)
    {
        return;
    }

    actor_move(id);
}

void actor_disable(uint8_t id)
{
    Actor *a = actors + id;
    a->state = ACTOR_STATE_DISABLED;
    actor_hide(id);
}

void actor_enable(uint8_t id)
{
    Actor *a = actors + id;
    a->state = ACTOR_STATE_ENABLED;
    actor_move(id);
}
