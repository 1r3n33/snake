#include <gb/gb.h>
#include <rand.h>
#include "background.h"
#include "camera.h"
#include "bonus.h"
#include "state.h"

#define BONUS_STATE_INVISIBLE 0U
#define BONUS_STATE_SPAWNING 1U
#define BONUS_STATE_VISIBLE 2U

typedef struct Bonus
{
    uint8_t x;
    uint8_t y;
    uint8_t state;
    uint8_t timer;
    uint8_t spawnzone_top;    // rectangle where bonus can spawn
    uint8_t spawnzone_bottom; //
    uint8_t spawnzone_left;   //
    uint8_t spawnzone_right;  //
} Bonus;

Bonus bonus;

void bonus_init()
{
    bonus.x = 8;
    bonus.y = 8;
    bonus.state = BONUS_STATE_INVISIBLE;
    bonus.timer = 255;
    bonus_set_spawn_zone_rect(
        BACKGROUND_SAFE_ZONE_TOP,
        BACKGROUND_SAFE_ZONE_BOTTOM,
        BACKGROUND_SAFE_ZONE_LEFT,
        BACKGROUND_SAFE_ZONE_RIGHT);

    set_sprite_tile(8, 32);
    set_sprite_tile(9, 33);
    set_sprite_tile(10, 34);
    set_sprite_tile(11, 35);
}

void bonus_update_invisible()
{
    bonus.timer--;
    if (!bonus.timer)
    {
        bonus.state = BONUS_STATE_SPAWNING;
    }
}

void bonus_update_spawning()
{
    // TODO: Small optimization by pre-computing right-left and bottom-top
    uint8_t x = bonus.spawnzone_left + (rand() % (bonus.spawnzone_right - bonus.spawnzone_left));
    uint8_t y = bonus.spawnzone_top + (rand() % (bonus.spawnzone_bottom - bonus.spawnzone_top));

    // Could we just use background_peek_1x1 if x and y were multiple of 2?
    if (!background_peek_2x2(x, y))
    {
        bonus.x = x;
        bonus.y = y;
        bonus.state = BONUS_STATE_VISIBLE;
        bonus.timer = 255;
    }
}

void bonus_update_visible(SnakeNode *head)
{
    uint8_t bonus_left = bonus.x;
    uint8_t bonus_right = bonus.x + 2;
    uint8_t bonus_top = bonus.y;
    uint8_t bonus_bottom = bonus.y + 2;

    uint8_t head_left = head->x;
    uint8_t head_right = head->x + 2;
    uint8_t head_top = head->y;
    uint8_t head_bottom = head->y + 2;

    if (bonus_left < head_right && bonus_right > head_left && bonus_top < head_bottom && bonus_bottom > head_top)
    {
        State *state = state_get();
        state->score++;

        snake_lock_tail(2U);

        move_sprite(8, 0, 0);
        move_sprite(9, 0, 0);
        move_sprite(10, 0, 0);
        move_sprite(11, 0, 0);
        bonus.state = BONUS_STATE_INVISIBLE;
        bonus.timer = 255U;
    }
    else
    {
        // Always visibile at the moment.
        // bonus.timer--;
        if (!bonus.timer)
        {
            move_sprite(8, 0, 0);
            move_sprite(9, 0, 0);
            move_sprite(10, 0, 0);
            move_sprite(11, 0, 0);
            bonus.state = BONUS_STATE_INVISIBLE;
            bonus.timer = 255U;
        }
        else
        {
            uint16_t bonus_left = bonus.x * 8U;
            uint16_t bonus_right = bonus_left + 16U;
            uint16_t bonus_top = bonus.y * 8U;
            uint16_t bonus_bottom = bonus_top + 16U;

            Camera *cam = camera_get();
            uint16_t cam_left = cam->sx;
            uint16_t cam_right = cam_left + DEVICE_SCREEN_PX_WIDTH;
            uint16_t cam_top = cam->sy;
            uint16_t cam_bottom = cam_top + DEVICE_SCREEN_PX_HEIGHT;

            if (bonus_left < cam_right && bonus_right > cam_left && bonus_top < cam_bottom && bonus_bottom > cam_top)
            {
                uint8_t x = DEVICE_SPRITE_PX_OFFSET_X + bonus_left - (cam->sx % 256U);
                uint8_t y = DEVICE_SPRITE_PX_OFFSET_Y + bonus_top - (cam->sy % 256U);
                move_sprite(8, x, y);
                move_sprite(9, x, y + 8);
                move_sprite(10, x + 8, y);
                move_sprite(11, x + 8, y + 8);
            }
            else
            {
                move_sprite(8, 0, 0);
                move_sprite(9, 0, 0);
                move_sprite(10, 0, 0);
                move_sprite(11, 0, 0);
            }
        }
    }
}

void bonus_update(SnakeNode *head)
{
    switch (bonus.state)
    {
    case BONUS_STATE_INVISIBLE:
        bonus_update_invisible();
        break;

    case BONUS_STATE_SPAWNING:
        bonus_update_spawning();
        break;

    case BONUS_STATE_VISIBLE:
        bonus_update_visible(head);
        break;
    }
}

void bonus_set_spawn_zone_rect(uint8_t top, uint8_t bottom, uint8_t left, uint8_t right)
{
    bonus.spawnzone_top = top;
    bonus.spawnzone_bottom = bottom;
    bonus.spawnzone_left = left;
    bonus.spawnzone_right = right;
}
