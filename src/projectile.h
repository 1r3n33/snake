#ifndef PROJECTILE_H
#define PROJECTILE_H

#include <gb/gb.h>

void projectile_init_all();
void projectile_init(const uint8_t id);

void projectile_update_all();
void projectile_update(const uint8_t id);

void projectile_spawn();

#endif // PROJECTILE_H
