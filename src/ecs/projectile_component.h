#ifndef ECS_PROJECTILE_COMPONENT_H
#define ECS_PROJECTILE_COMPONENT_H

#include <gb/gb.h>

typedef struct ProjectileComponent
{
    uint8_t enabled;
    int8_t dx, dy; // in px/frame
} ProjectileComponent;

#define PROJECTILE_COMPONENT_MAX_COUNT 4U

extern ProjectileComponent proj_components[PROJECTILE_COMPONENT_MAX_COUNT];

#endif // ECS_PROJECTILE_COMPONENT_H
