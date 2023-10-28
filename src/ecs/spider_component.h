#ifndef ECS_SPIDER_COMPONENT_H
#define ECS_SPIDER_COMPONENT_H

#include <gb/gb.h>

// Visibility zone
// Spiders share a unique gfx component.
// Each spider has a non-overlapping visibility zone.
// When a zone becomes visible, the system activates the spider.
typedef struct Zone
{
    uint8_t l;
    uint8_t r;
    uint8_t t;
    uint8_t b;
} Zone;

typedef struct SpiderComponent
{
    uint8_t enabled;
    Zone zone;
} SpiderComponent;

#define SPIDER_COMPONENT_MAX_COUNT 8U

extern SpiderComponent spd_components[SPIDER_COMPONENT_MAX_COUNT];

#endif // ECS_SPIDER_COMPONENT_H
