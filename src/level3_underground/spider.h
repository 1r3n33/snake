#ifndef UNDERGROUND_SPIDER_H
#define UNDERGROUND_SPIDER_H

#include <gb/gb.h>

void spider_init_all() BANKED;
void spider_init(const uint8_t id) BANKED;

void spider_update_all() BANKED;
void spider_update(const uint8_t id) BANKED;

void spider_spawn(const uint16_t x, const uint16_t y) BANKED;

#endif // UNDERGROUND_SPIDER_H
