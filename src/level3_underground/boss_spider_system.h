#pragma once
#include <gb/gb.h>

void bspd_sys_init() BANKED;

void bspd_sys_process() BANKED;

uint8_t bspd_sys_is_initialized() BANKED;
void bspd_sys_set_initialized() BANKED;
