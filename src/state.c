#include "state.h"

State state;

void state_init()
{
    state.tail_locked = 0;
}

State *state_get()
{
    return &state;
}
