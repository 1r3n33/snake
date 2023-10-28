#ifndef ECS_SPIDER_SYSTEM_H
#define ECS_SPIDER_SYSTEM_H

#include "collision_component.h"
#include "graphic_component.h"

void spd_sys_init(GraphicComponent *gfx, CollisionComponent *col);

void spd_sys_process();

#endif // ECS_SPIDER_SYSTEM_H
