#ifndef ECS_COLLISION_SYSTEM_H
#define ECS_COLLISION_SYSTEM_H

#include <gb/gb.h>

void col_sys_init();

void col_sys_process();

uint8_t col_sys_get_data();

void col_sys_bkg_collider_enable(uint8_t id);
void col_sys_bkg_collider_disable(uint8_t id);

void col_sys_c2c_collider_enable(uint8_t id);
void col_sys_c2c_collider_disable(uint8_t id);
void col_sys_c2c_collider_candidates(uint8_t id, uint8_t mask);

void col_sys_c2c_collidee_enable(uint8_t id);
void col_sys_c2c_collidee_disable(uint8_t id);

#endif // ECS_COLLISION_SYSTEM_H
