#ifndef ECS_COLLISION_COMPONENT_H
#define ECS_COLLISION_COMPONENT_H

#include <gb/gb.h>

typedef struct CollisionComponent
{
    uint16_t bb[4];          // bounding box (lrtb)
    uint8_t data;            // Custom data passed for collision callback
    void (*on_bkg_col_cb)(); // on background collision callback
    void (*on_c2c_col_cb)(); // on component to component collision callback
} CollisionComponent;

#define COLLISION_COMPONENT_MAX_COLLIDER_COUNT 4U
#define COLLISION_COMPONENT_MAX_COLLIDEE_COUNT 4U

extern CollisionComponent col_colliders[COLLISION_COMPONENT_MAX_COLLIDER_COUNT];
extern CollisionComponent col_collidees[COLLISION_COMPONENT_MAX_COLLIDEE_COUNT];

inline void col_set_bounding_box(CollisionComponent *col, uint16_t left, uint16_t right, uint16_t top, uint16_t bottom)
{
    col->bb[0U] = left;
    col->bb[1U] = right;
    col->bb[2U] = top;
    col->bb[3U] = bottom;
}

inline void col_move(CollisionComponent *col, int8_t dx, int8_t dy)
{
    col->bb[0U] += dx;
    col->bb[1U] += dx;
    col->bb[2U] += dy;
    col->bb[3U] += dy;
}

#endif // ECS_COLLISION_COMPONENT_H
