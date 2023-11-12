#pragma bank 4
#include <string.h>
#include <gb/gb.h>
#include "boss_spider_component.h"
#include "boss_spider_system.h"
#include "../ecs/collision_system.h" // TODO: Not great to have a dependency on the other system!
#include "../ecs/graphic_component.h"

typedef struct BossSpiderSystem
{
    uint8_t enabled;
    uint8_t initialized;
    uint8_t alive;
} BossSpiderSystem;

BossSpiderSystem bspd_sys;

void bspd_sys_init() BANKED
{
    memset(bspd_components, 0U, sizeof(bspd_components));

    bspd_sys.enabled = 1U;
    bspd_sys.initialized = 0U;
    bspd_sys.alive = 1U;
}

void bspd_sys_process() BANKED
{
    if (!bspd_sys.enabled)
    {
        for (uint8_t i = 0; i < BOSS_SPIDER_COMPONENT_MAX_COUNT; i++)
        {
            BossSpiderComponent *comp = bspd_components + i;
            GraphicComponent *gfx = gfx_components + comp->gfx_uid;
            gfx_disable(gfx);
        }
        return;
    }

    uint8_t alive_count = 0U;

    // Process even if disabled (for blink)
    // TODO: This should take the enable flag into consideration.
    for (uint8_t i = 0; i < BOSS_SPIDER_COMPONENT_MAX_COUNT; i++)
    {
        BossSpiderComponent *comp = bspd_components + i;
        if (comp->blink)
        {
            uint8_t blink = (--comp->blink) & 3U;
            GraphicComponent *gfx = gfx_components + comp->gfx_uid;
            if (blink)
            {
                gfx_disable(gfx);
            }
            else
            {
                gfx_enable(gfx); // enabled on last blink frame
            }
            col_sys_c2c_collidee_disable(i + 1U);
        }
        else
        {
            if (comp->enabled)
            {
                col_sys_c2c_collidee_enable(i + 1U);
            }
        }

        alive_count += comp->enabled;
    }

    if (bspd_sys.alive)
    {
        if (alive_count == 0)
        {
            // Set final blink
            bspd_sys.alive = 0U;
            bspd_sys.enabled = 60U;
            for (uint8_t i = 0; i < BOSS_SPIDER_COMPONENT_MAX_COUNT; i++)
            {
                BossSpiderComponent *comp = bspd_components + i;
                comp->blink = 60U;
            }
        }
    }
    else
    {
        bspd_sys.enabled--;
    }
}

uint8_t bspd_sys_is_initialized() BANKED
{
    return bspd_sys.initialized;
}

void bspd_sys_set_initialized() BANKED
{
    bspd_sys.initialized = 1U;
}
