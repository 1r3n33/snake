#ifndef ECS_PROJECTILE_SYSTEM_H
#define ECS_PROJECTILE_SYSTEM_H

#include <gb/gb.h>
#include "collision_component.h"
#include "graphic_component.h"

void proj_sys_init(GraphicComponent *graphics, CollisionComponent *collisions);

void proj_sys_process(uint8_t pressed);

#endif // ECS_PROJECTILE_SYSTEM_H
