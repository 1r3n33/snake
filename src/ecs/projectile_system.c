#include <string.h>
#include <gb/gb.h>
#include "projectile_component.h"
#include "projectile_system.h"
#include "collision_system.h" // TODO: Not great to have a dependency on the other system!
#include "../snake.h"         // TODO: Will migrate to ECS

#define PROJ_H_TILE_ID 40U // TODO: Move in graphics.h
#define PROJ_V_TILE_ID 41U // TODO: Move in graphics.h
#define PROJ_MAX_COUNT 4U
#define PROJ_SPAWN_DELAY 30U // in frames
#define PROJ_SPEED 4         // px/frame

const uint8_t proj_tile_id_by_direction[4U] = {PROJ_H_TILE_ID, PROJ_H_TILE_ID, PROJ_V_TILE_ID, PROJ_V_TILE_ID};
const uint8_t proj_sprite_prop_by_direction[4U] = {S_FLIPY, 0U, S_FLIPX, 0U};
const int8_t proj_x_offset_by_direction[4U] = {-4, -4, 0, 0};
const int8_t proj_y_offset_by_direction[4U] = {0, 0, -4, -4};
const int8_t proj_dx_by_direction[4U] = {0, 0, -PROJ_SPEED, PROJ_SPEED};
const int8_t proj_dy_by_direction[4U] = {-PROJ_SPEED, PROJ_SPEED, 0, 0};

typedef struct ProjectileSystem
{
    // System data
    uint8_t spawn_delay; // delay before spawn (in frames).
    // Components data
    GraphicComponent *graphics;
    CollisionComponent *collisions;
} ProjectileSystem;

ProjectileSystem proj_sys;

void proj_sys_init(GraphicComponent *graphics, CollisionComponent *collisions)
{
    memset(proj_components, 0U, sizeof(proj_components));

    proj_sys.spawn_delay = 0U;
    proj_sys.graphics = graphics;
    proj_sys.collisions = collisions;
}

void proj_sys_process(uint8_t pressed)
{
    // Process delay
    if (proj_sys.spawn_delay)
    {
        proj_sys.spawn_delay--;
    }

    // Process spawn request
    if ((pressed & J_A) && (proj_sys.spawn_delay == 0U))
    {
        for (uint8_t i = 0U; i < PROJECTILE_COMPONENT_MAX_COUNT; i++)
        {
            ProjectileComponent *proj = proj_components + i;
            if (!proj->enabled)
            {
                proj->enabled = 1U;

                SnakeNode *head = snake_get_head();
                uint16_t x = (head->x * 8U) + head->offset_x + proj_x_offset_by_direction[head->in];
                uint16_t y = (head->y * 8U) + head->offset_y + proj_y_offset_by_direction[head->in];

                proj->dx = proj_dx_by_direction[head->in];
                proj->dy = proj_dy_by_direction[head->in];

                GraphicComponent *gfx = proj_sys.graphics + i;
                gfx_enable(gfx);
                gfx_set_xy(gfx, x, y);
                gfx_set_tile_ids(gfx, proj_tile_id_by_direction + head->in);
                gfx_set_tile_props(gfx, proj_sprite_prop_by_direction + head->in);

                CollisionComponent *col = proj_sys.collisions + i;
                col_set_bounding_box(col, x, x + 8, y, y + 8);
                col_sys_bkg_collider_enable(i);
                col_sys_c2c_collider_enable(i);
                
                proj_sys.spawn_delay = PROJ_SPAWN_DELAY;
                break;
            }
        }
    }

    // Move projectiles
    for (uint8_t i = 0U; i < PROJECTILE_COMPONENT_MAX_COUNT; i++)
    {
        ProjectileComponent *proj = proj_components + i;
        if (proj->enabled)
        {
            int8_t dx = proj->dx;
            int8_t dy = proj->dy;

            GraphicComponent *gfx = proj_sys.graphics + i;
            gfx_move(gfx, dx, dy);

            CollisionComponent *col = proj_sys.collisions + i;
            col_move(col, dx, dy);
        }
    }
}
