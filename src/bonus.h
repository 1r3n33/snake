#ifndef BONUS_H
#define BONUS_H

#include "snake.h"

void bonus_init();
void bonus_update(SnakeNode *head);
void bonus_set_spawn_zone_rect(uint8_t top, uint8_t bottom, uint8_t left, uint8_t right);

#endif // BONUS_H
