#include <gb/gb.h>
#include <rand.h>
#include "bonus.h"
#include "camera.h"
#include "direction.h"
#include "eyes.h"
#include "game.h"
#include "snake.h"
#include "tiles_copy.h"
#include "trigger.h"

void vblank_wait_in()
{
    // Used as a frame start marker.
    __asm__("halt");

    // Wait for VBLANK to get access to the VRAM.
    while (((volatile uint8_t)STAT_REG & (uint8_t)3U) != (uint8_t)1U)
        ;
}

void vblank_wait_out()
{
#if 1
    // Wait for VBLANK to end.
    // TODO: Remove this wait? Can't find it in the profiler but hit with with a breakpoint...
    // This wait has no real purpose. It can be desirable to start the new frame ASAP.
    // This could help with heavy frames 0 and 8 that update the snake tiles.
    while (((volatile uint8_t)STAT_REG & (uint8_t)3U) == (uint8_t)1U)
        ;
#endif
}

void vblank_update(uint8_t frame)
{
    vblank_wait_in();

    tc_apply_snake();

    if (frame == 3 || frame == 11)
    {
        tc_apply_row();
    }
    if (frame == 4 || frame == 12)
    {
        tc_apply_column();
    }
    camera_apply();

    vblank_wait_out();
}

uint8_t game_loop()
{
    uint8_t res = TRIGGER_CONTINUE;

    uint8_t frame = 0;
    uint8_t pressedOnce = 0;
    uint8_t lastPressed = 0;

    while (1)
    {
        vblank_update(frame);

        const uint8_t pressed = joypad();
        if (pressed)
        {
            lastPressed = pressed;
        }

        if (pressedOnce)
        {
            SnakeNode *head = snake_get_head();

            // At frame 16 (or 0) a new cycle begins.
            // Snake advances using the last pressed direction.
            if (frame == 16)
            {
                frame = 0;

                if (lastPressed & J_UP)
                {
                    snake_update(DIRECTION_NORTH);
                }
                else if (lastPressed & J_DOWN)
                {
                    snake_update(DIRECTION_SOUTH);
                }
                else if (lastPressed & J_LEFT)
                {
                    snake_update(DIRECTION_WEST);
                }
                else if (lastPressed & J_RIGHT)
                {
                    snake_update(DIRECTION_EAST);
                }
                else
                {
                    // If no direction is pressed, keep the current one.
                    snake_update(head->in);
                }
            }
            else
            {
                snake_tick(frame);
            }
            camera_move(head);

            eyes_update();
            bonus_update(head);

            res = trigger_update();

            frame++;
        }
        else
        {
            pressedOnce = pressed & (J_UP | J_DOWN | J_LEFT | J_RIGHT);
            initrand(((uint16_t)DIV_REG << 8) | pressed);
            frame = 1;
        }

        if (res != TRIGGER_CONTINUE)
        {
            return res;
        }
    }
}
