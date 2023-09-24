#include "background.h"
#include "camera.h"
#include "projectile.h"

#define PROJECTILE_SPRITE_0 28U

#define PROJECTILE_MAX_COUNT 4U

#define PROJECTILE_SPAWN_DELAY 30U // in frames

#define PROJECTILE_SPEED 4 // px/frame

typedef struct Projectile
{
    uint8_t state; // 0 disabled, 1 enabled
    uint16_t x;    // x pos
    uint16_t y;    // y pos
    int8_t dx;     // dx
    int8_t dy;     // dy
} Projectile;

uint8_t projectile_spawn_deplay = 0U;
Projectile projectiles[PROJECTILE_MAX_COUNT];

// NSWE
const uint8_t projectile_tile_id_by_direction[4U] = {40U, 40U, 41U, 41U};
const uint8_t projectile_sprite_prop_by_direction[4U] = {S_FLIPY, 0U, S_FLIPX, 0U};
const int8_t projectile_x_offset_by_direction[4U] = {-4, -4, 0, 0};
const int8_t projectile_y_offset_by_direction[4U] = {0, 0, -4, -4};
const int8_t projectile_dx_by_direction[4U] = {0, 0, -PROJECTILE_SPEED, PROJECTILE_SPEED};
const int8_t projectile_dy_by_direction[4U] = {-PROJECTILE_SPEED, PROJECTILE_SPEED, 0, 0};

void projectile_init_all()
{
    projectile_spawn_deplay = 0U;
    for (uint8_t i = 0U; i < PROJECTILE_MAX_COUNT; i++)
    {
        projectile_init(i);
    }
}

void projectile_init(const uint8_t id)
{
    Projectile *p = projectiles + id;
    p->state = 0U;
    p->x = 0;
    p->y = 0;

    move_sprite(PROJECTILE_SPRITE_0 + id, 0U, 0U);
}

void projectile_update_all()
{
    if (projectile_spawn_deplay)
    {
        projectile_spawn_deplay--;
    }

    for (uint8_t i = 0U; i < PROJECTILE_MAX_COUNT; i++)
    {
        projectile_update(i);
    }
}

void projectile_update(const uint8_t id)
{
    Projectile *p = projectiles + id;
    if (p->state)
    {
        p->x += p->dx;
        p->y += p->dy;

        // Check collision against background.
        // Resolve background tile using the center of the sprite.
        uint8_t cx = (p->x + 4U) / 8U;
        uint8_t cy = (p->y + 4U) / 8U;
        if (background_peek_1x1(cx, cy))
        {
            p->state = 0U;
            move_sprite(PROJECTILE_SPRITE_0 + id, 0U, 0U);
            return;
        }

        uint16_t proj_left = p->x;
        uint16_t proj_right = p->x + 8U;
        uint16_t proj_top = p->y;
        uint16_t proj_bottom = p->y + 8U;

        Camera *cam = camera_get();
        uint16_t cam_left = cam->sx;
        uint16_t cam_right = cam_left + DEVICE_SCREEN_PX_WIDTH;
        uint16_t cam_top = cam->sy;
        uint16_t cam_bottom = cam_top + DEVICE_SCREEN_PX_HEIGHT;

        if (proj_left < cam_right && proj_right > cam_left && proj_top < cam_bottom && proj_bottom > cam_top)
        {
            uint8_t x = DEVICE_SPRITE_PX_OFFSET_X + proj_left - (cam->sx % 256U);
            uint8_t y = DEVICE_SPRITE_PX_OFFSET_Y + proj_top - (cam->sy % 256U);
            move_sprite(PROJECTILE_SPRITE_0 + id, x, y);
        }
        else
        {
            move_sprite(PROJECTILE_SPRITE_0 + id, 0U, 0U);
        }
    }
    else
    {
        move_sprite(PROJECTILE_SPRITE_0 + id, 0U, 0U);
    }
}

void projectile_spawn()
{
    if (projectile_spawn_deplay > 0U)
    {
        return;
    }

    for (uint8_t id = 0U; id < PROJECTILE_MAX_COUNT; id++)
    {
        Projectile *p = projectiles + id;
        if (!p->state)
        {
            SnakeNode *head = snake_get_head();
            p->state = 1U;
            p->x = (head->x * 8U) + head->offset_x + projectile_x_offset_by_direction[head->in];
            p->y = (head->y * 8U) + head->offset_y + projectile_y_offset_by_direction[head->in];
            p->dx = projectile_dx_by_direction[head->in];
            p->dy = projectile_dy_by_direction[head->in];
            set_sprite_tile(PROJECTILE_SPRITE_0 + id, projectile_tile_id_by_direction[head->in]);
            set_sprite_prop(PROJECTILE_SPRITE_0 + id, projectile_sprite_prop_by_direction[head->in]);

            projectile_spawn_deplay = PROJECTILE_SPAWN_DELAY;
            break;
        }
    }
}

// Return after the fisrt valid collision...
uint8_t projectile_check_collision(const uint16_t top, const uint16_t left, const uint16_t bottom, const uint16_t right)
{
    for (uint8_t id = 0U; id < PROJECTILE_MAX_COUNT; id++)
    {
        Projectile *p = projectiles + id;
        if (p->state)
        {
            uint16_t proj_left = p->x;
            uint16_t proj_right = p->x + 8U;
            uint16_t proj_top = p->y;
            uint16_t proj_bottom = p->y + 8U;

            if (proj_left < right && proj_right > left && proj_top < bottom && proj_bottom > top)
            {
                p->state = 0;
                return 1;
            }
        }
    }
    return 0;
}
