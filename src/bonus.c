#include <gb/gb.h>
#include <rand.h>
#include "background.h"
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
} Bonus;

Bonus bonus;

void bonus_init()
{
    bonus.x = 8;
    bonus.y = 8;
    bonus.state = BONUS_STATE_INVISIBLE;
    bonus.timer = 255;

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
    uint8_t x = BACKGROUND_SAFE_ZONE_LEFT + (rand() % (BACKGROUND_SAFE_ZONE_RIGHT - BACKGROUND_SAFE_ZONE_LEFT));
    uint8_t y = BACKGROUND_SAFE_ZONE_TOP + (rand() % (BACKGROUND_SAFE_ZONE_BOTTOM - BACKGROUND_SAFE_ZONE_TOP));
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
        state_get()->tail_locked = 1;
        move_sprite(8, 0, 0);
        move_sprite(9, 0, 0);
        move_sprite(10, 0, 0);
        move_sprite(11, 0, 0);
        bonus.state = BONUS_STATE_INVISIBLE;
        bonus.timer = 255;
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
            bonus.timer = 255;
        }
        else
        {
            uint16_t x = DEVICE_SPRITE_PX_OFFSET_X + (bonus.x * 8) - SCX_REG;
            uint16_t y = DEVICE_SPRITE_PX_OFFSET_Y + (bonus.y * 8) - SCY_REG;
            move_sprite(8, x, y);
            move_sprite(9, x, y + 8);
            move_sprite(10, x + 8, y);
            move_sprite(11, x + 8, y + 8);
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
