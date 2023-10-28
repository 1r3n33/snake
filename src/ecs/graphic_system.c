#include <string.h>
#include <gb/gb.h>
#include "graphic_component.h"
#include "graphic_system.h"
#include "../camera.h"

void gfx_sys_init()
{
    memset(gfx_components, 0U, sizeof(gfx_components));
}

void gfx_sys_process()
{
    Camera *cam = camera_get();
    uint16_t cam_left = cam->sx;
    uint16_t cam_right = cam_left + DEVICE_SCREEN_PX_WIDTH;
    uint16_t cam_top = cam->sy;
    uint16_t cam_bottom = cam_top + DEVICE_SCREEN_PX_HEIGHT;

    for (uint8_t i = 0U; i < GRAPHIC_COMPONENT_MAX_COUNT; i++)
    {
        GraphicComponent *gfx = gfx_components + i;
        if (gfx->enabled)
        {
            uint16_t gfx_left = gfx->x + gfx->bb[0U];
            uint16_t gfx_right = gfx->x + gfx->bb[1U];
            uint16_t gfx_top = gfx->y + gfx->bb[2U];
            uint16_t gfx_bottom = gfx->y + gfx->bb[3U];

            if (gfx_left <= cam_right && gfx_right >= cam_left && gfx_top <= cam_bottom && gfx_bottom >= cam_top)
            {
                uint8_t x = DEVICE_SPRITE_PX_OFFSET_X + gfx_left - (cam->sx % 256U);
                uint8_t y = DEVICE_SPRITE_PX_OFFSET_Y + gfx_top - (cam->sy % 256U);

                uint8_t i = gfx->id;
                uint8_t c = gfx->count;
                int8_t *p = gfx->xy_offsets;
                while (c)
                {
                    uint8_t tx = x + *p++;
                    uint8_t ty = y + *p++;
                    move_sprite(i, tx, ty);
                    c--;
                    i++;
                }
            }
            else
            {
                uint8_t i = gfx->id;
                uint8_t c = gfx->count;
                while (c)
                {
                    move_sprite(i, 0U, 0U);
                    c--;
                    i++;
                }
            }
        }
    }
}
