#ifndef TRIGGER_H
#define TRIGGER_H

#include <gb/gb.h>

#define TRIGGER_CONTINUE 0
#define TRIGGER_NEXT_TRIGGER 1
#define TRIGGER_NEXT_LEVEL 2
#define TRIGGER_RESTART_LEVEL 3

typedef struct Trigger
{
    // Check trigger condition, return > 0 when completed.
    uint8_t (*check)(void);

    struct Trigger *next;
} Trigger;

void trigger_init(Trigger *trigger);
uint8_t trigger_update();

#endif // TRIGGER_H
