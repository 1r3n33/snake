#ifndef BACKGROUND_H
#define BACKGROUND_H

#include "snake.h"

#define BACKGROUND_WIDTH 32U
#define BACKGROUND_HEIGHT 32U

#define BACKGROUND_SAFE_ZONE_LEFT 2U
#define BACKGROUND_SAFE_ZONE_RIGHT BACKGROUND_WIDTH - 2U
#define BACKGROUND_SAFE_ZONE_TOP 2U
#define BACKGROUND_SAFE_ZONE_BOTTOM BACKGROUND_HEIGHT - 2U

uint8_t *background_get();
void background_init();
void background_update(uint16_t offset, uint8_t v);
uint8_t background_check_collision(SnakeNode *head);
uint8_t background_peek_1x1(uint8_t x, uint8_t y);
uint8_t background_peek_2x2(uint8_t x, uint8_t y);

#endif // BACKGROUND_H
