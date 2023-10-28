#include <string.h>
#include <gb/gb.h>
#include "collision_component.h"
#include "collision_system.h"
#include "../background.h"

typedef struct CollisionSystem
{
    // Background collision
    uint8_t bkg_collider_enabled[COLLISION_COMPONENT_MAX_COLLIDER_COUNT];

    // Component to component collision
    //
    // Per collider mask to know if collider is enabled (0xFF) or not (0x00).
    uint8_t c2c_collider_enabled[COLLISION_COMPONENT_MAX_COLLIDER_COUNT];
    // Per collider mask to know which collidees need to be checked (aka collision pairs).
    uint8_t c2c_collider_candidates[COLLISION_COMPONENT_MAX_COLLIDER_COUNT];
    // Mask to know which collidees are enabled.
    uint8_t c2c_collidee_enabled;
    //
    // Example:
    //
    // collider_enabled    collider_candidates
    //             0xFF                   0x00    (collider 0 is enabled but has no colidee)
    //             0x00                   0x03    (collider 1 is disabled but collides with collidees 0 and 1)
    //             0xFF                   0x06    (collider 2 is enabled and collides with collidees 1 and 2)
    //             0xFF                   0xFF    (collider 3 is enabled and collides with all collidees)
    //
    //                        collidee_enabled
    //                                    0x0F    (collidees 0, 1, 2, 3 are enabled)
    //
    // Result by applying (collider_enabled[i] & collidee_candidates[i] & collidee_enabled)
    //
    //    0x00 (no collision check for collider 0)
    //    0x00 (no collision check for collider 1)
    //    0x06 (check colidees 1 and 2 for collider 2)
    //    0x0f (check colidees 0, 1, 2, 3 for collider 3)
    //

    // Since there is a compiler bug with passing parameter to callbaks,
    // store current collision data globally.
    uint8_t data;

} CollisionSystem;

CollisionSystem col_sys;

inline void check_collision(CollisionComponent *collider, CollisionComponent *collidee)
{
    if (collider->bb[0] < collidee->bb[1] &&
        collider->bb[1] > collidee->bb[0] &&
        collider->bb[2] < collidee->bb[3] &&
        collider->bb[3] > collidee->bb[2])
    {
        col_sys.data = collider->data;
        collider->on_c2c_col_cb();
        col_sys.data = collidee->data;
        collidee->on_c2c_col_cb();
    }
}

void col_sys_init()
{
    memset(col_colliders, 0U, sizeof(col_colliders));
    memset(col_collidees, 0U, sizeof(col_collidees));
    memset(&col_sys, 0U, sizeof(col_sys));
}

void col_sys_process()
{
    // Background collision
    for (uint8_t i = 0U; i < COLLISION_COMPONENT_MAX_COLLIDER_COUNT; i++)
    {
        if (col_sys.bkg_collider_enabled[i])
        {
            CollisionComponent *col = col_colliders + i;

            // Check collision against the top-left corner.
            uint8_t x = col->bb[0] / 8U;
            uint8_t y = col->bb[2] / 8U;
            uint8_t hit = background_peek_1x1(x, y);
            if (hit)
            {
                col_sys.data = col->data;
                col->on_bkg_col_cb();
            }
        }
    }

    // Component to component collision
    for (uint8_t i = 0U; i < COLLISION_COMPONENT_MAX_COLLIDER_COUNT - 1U; i++)
    {
        // TODO: this mask could be pre-computed after each enable/disable
        uint8_t mask = col_sys.c2c_collider_enabled[i] & col_sys.c2c_collider_candidates[i] & col_sys.c2c_collidee_enabled;
        if (mask)
        {
            CollisionComponent *col = col_colliders + i;
            // TODO: Implement early-out when a collision happens.
            if (mask & 0x01U)
                check_collision(col, col_collidees + 0U);
            if (mask & 0x02U)
                check_collision(col, col_collidees + 1U);
            if (mask & 0x04U)
                check_collision(col, col_collidees + 2U);
            if (mask & 0x08U)
                check_collision(col, col_collidees + 3U);
#if COLLISION_COMPONENT_MAX_COLLIDEE_COUNT > 4
#error Update component to component collision check
#endif
        }
    }
}

uint8_t col_sys_get_data()
{
    return col_sys.data;
}

void col_sys_bkg_collider_enable(uint8_t id)
{
    col_sys.bkg_collider_enabled[id] = 1U;
}

void col_sys_bkg_collider_disable(uint8_t id)
{
    col_sys.bkg_collider_enabled[id] = 0U;
}

void col_sys_c2c_collider_enable(uint8_t id)
{
    col_sys.c2c_collider_enabled[id] = 0xFFU;
}

void col_sys_c2c_collider_disable(uint8_t id)
{
    col_sys.c2c_collider_enabled[id] = 0x00U;
}

void col_sys_c2c_collider_candidates(uint8_t id, uint8_t mask)
{
    col_sys.c2c_collider_candidates[id] = mask;
}

void col_sys_c2c_collidee_enable(uint8_t id)
{
    col_sys.c2c_collidee_enabled |= (1U << id);
}

void col_sys_c2c_collidee_disable(uint8_t id)
{
    col_sys.c2c_collidee_enabled &= ~(1U << id);
}
