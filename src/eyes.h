#ifndef EYES_H
#define EYES_H

#include <gb/gb.h>
#include "snake.h"

void eyes_init();
void eyes_move(SnakeNode *head);
void eyes_ko();

#endif // EYES_H
