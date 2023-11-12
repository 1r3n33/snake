#ifndef ECS_SPIDER_SYSTEM_H
#define ECS_SPIDER_SYSTEM_H

#include "../ecs/collision_component.h"
#include "../ecs/graphic_component.h"

void spd_sys_init(GraphicComponent *gfx, CollisionComponent *col) BANKED;

void spd_sys_process() BANKED;

#endif // ECS_SPIDER_SYSTEM_H
