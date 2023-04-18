#ifndef STATE_H
#define STATE_H

#include <gb/gb.h>

typedef struct State_t
{
    uint8_t tail_locked;
    uint8_t score;
} State;

void state_init();
State *state_get();

#endif // STATE_H
