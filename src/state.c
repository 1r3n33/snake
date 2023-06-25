#include "state.h"

State state;

void state_init()
{
    state.score = 0;
    state.ko = 0;
}

State *state_get()
{
    return &state;
}
