#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <gb/gb.h>

void projectile_init_all();
void projectile_init(const uint8_t id);

void projectile_update_all();
void projectile_update(const uint8_t id);

void projectile_spawn();

uint8_t projectile_check_collision(const uint16_t top, const uint16_t left, const uint16_t bottom, const uint16_t right);

#endif // PROJECTILE_H
