#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "snake.h"

#define BACKGROUND_WIDTH 32U
#define BACKGROUND_HEIGHT 32U

uint8_t *background_get();
void background_init();
void background_update(uint16_t offset, uint8_t v);
uint8_t background_check_collision(SnakeNode *head);

#endif // BACKGROUND_H
