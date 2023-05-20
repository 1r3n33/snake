#ifndef GAME_H
#define GAME_H

#include <gb/gb.h>

int8_t game_loop(void (*loop_update)(void), int8_t (*loop_check)(void));

#endif // GAME_H
