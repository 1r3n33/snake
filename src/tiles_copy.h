#ifndef TILES_COPY_H
#define TILES_COPY_H

#include <gb/gb.h>

void tc_init();
void tc_reset();

void tiles_copy_push(uint16_t offset, uint8_t v);
void tiles_copy_run();

void tc_set_row(uint8_t x, uint8_t y, uint8_t cnt);
void tc_apply_row();
void tc_set_column(uint8_t x, uint8_t y, uint8_t cnt);
void tc_apply_column();

#endif // TILES_COPY_H
