#include <gb/gb.h>
#include "bonus.h"
#include "state.h"

#define BONUS_STATE_INVISIBLE 0U
#define BONUS_STATE_VISIBLE 1U

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

void bonus_update(SnakeNode *head)
{
    if (bonus.state == BONUS_STATE_VISIBLE)
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
            bonus.timer--;
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

    if (bonus.state == BONUS_STATE_INVISIBLE)
    {
        bonus.timer--;
        if (!bonus.timer)
        {
            bonus.state = BONUS_STATE_VISIBLE;
            bonus.timer = 255;
        }
    }
}
