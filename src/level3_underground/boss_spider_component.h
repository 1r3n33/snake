#pragma once

#include <gb/gb.h>

// BossSpiderComponent
// 4 eyes component (uid 0-3)
// 1 mouth component (uid 4) that can spawn spider

typedef struct BossSpiderComponent
{
    uint8_t uid;
    uint8_t enabled;
    uint8_t gfx_uid;
    uint8_t col_uid;
    uint8_t hit_count; // Number of hit before disabling the 
    uint8_t blink;
} BossSpiderComponent;

#define BOSS_SPIDER_COMPONENT_MAX_COUNT 4U

extern BossSpiderComponent bspd_components[BOSS_SPIDER_COMPONENT_MAX_COUNT];
