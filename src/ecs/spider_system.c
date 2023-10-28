#include <string.h>
#include "spider_component.h"
#include "spider_system.h"
#include "collision_system.h" // TODO: Not great to have a dependency on the other system!
#include "../camera.h"

// TODO: Could move to the same bank as underground.
// TODO: Add comments to explain shared gfx & col principles.

typedef struct SpiderSystem
{
    GraphicComponent *gfx;
    CollisionComponent *col;
} SpiderSystem;

SpiderSystem spd_sys;

#define ZONE_TO_COL_OFFSET_TOP 0U
#define ZONE_TO_COL_OFFSET_BOTTOM 4U
#define ZONE_TO_COL_OFFSET_LEFT 7U
#define ZONE_TO_COL_OFFSET_RIGHT 7U

void spd_sys_init(GraphicComponent *gfx, CollisionComponent *col)
{
    memset(spd_components, 0U, sizeof(spd_components));
    spd_sys.gfx = gfx;
    spd_sys.col = col;
}

void spd_sys_process()
{
    Camera *cam = camera_get();
    uint8_t cam_left = cam->sx / 8U;
    uint8_t cam_right = cam_left + DEVICE_SCREEN_WIDTH;
    uint8_t cam_top = cam->sy / 8U;
    uint8_t cam_bottom = cam_top + DEVICE_SCREEN_HEIGHT;

    for (uint8_t i = 0U; i < SPIDER_COMPONENT_MAX_COUNT; i++)
    {
        SpiderComponent *spd = spd_components + i;
        if (spd->enabled)
        {
            if (spd->zone.l < cam_right && spd->zone.r > cam_left && spd->zone.t < cam_bottom && spd->zone.b > cam_top)
            {
                gfx_enable(spd_sys.gfx);
                gfx_set_xy(spd_sys.gfx, (uint16_t)spd->zone.l * 8U, (uint16_t)spd->zone.t * 8U);

                spd_sys.col->data = i;
                col_set_bounding_box(spd_sys.col,
                                     ((uint16_t)spd->zone.l * 8U) + ZONE_TO_COL_OFFSET_LEFT,
                                     ((uint16_t)spd->zone.r * 8U) - ZONE_TO_COL_OFFSET_RIGHT,
                                     ((uint16_t)spd->zone.t * 8U) + ZONE_TO_COL_OFFSET_TOP,
                                     ((uint16_t)spd->zone.b * 8U) - ZONE_TO_COL_OFFSET_BOTTOM);
                col_sys_c2c_collidee_enable(0); // spider col is id 0
                return;
            }
        }
    }

    gfx_disable(spd_sys.gfx);
    col_sys_c2c_collidee_disable(0); // spider col is id 0
}
