#ifndef TILES_UPDATE_H
#define TILES_UPDATE_H

#include <gb/gb.h>
#include "snake.h"

void tu_apply(SnakeNode *node, uint8_t update_bkg);
void tu_apply_with_visibility_check(SnakeNode *node, uint8_t update_bkg);
void tu_apply_with_direction(SnakeNode *node, uint8_t dir, uint8_t update_bkg);

#endif // TILES_UPDATE_H