#ifndef TILES_COPY_H
#define TILES_COPY_H

#include <gb/gb.h>

void tiles_copy_init();
void tiles_copy_push(uint16_t offset, uint8_t v);
void tiles_copy_run();

#endif // TILES_COPY_H
