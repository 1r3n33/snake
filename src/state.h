#ifndef STATE_H
#define STATE_H

#include <gb/gb.h>

typedef struct State_t
{
    uint8_t score;
    uint8_t ko;
} State;

void state_init();
State *state_get();

#endif // STATE_H
