#include "trigger.h"

Trigger *trigger_current;

void trigger_init(Trigger *trigger)
{
    trigger_current = trigger;
}

uint8_t trigger_update()
{
    uint8_t res = TRIGGER_CONTINUE;

    if (trigger_current)
    {
        res = trigger_current->check();
        switch (res)
        {
        case TRIGGER_CONTINUE:
            return TRIGGER_CONTINUE;

        case TRIGGER_NEXT_TRIGGER:
            trigger_current = trigger_current->next;
            return trigger_current ? TRIGGER_CONTINUE : TRIGGER_NEXT_LEVEL;

        case TRIGGER_NEXT_LEVEL:
            trigger_current = 0;
            return TRIGGER_NEXT_LEVEL;

        case TRIGGER_RESTART_LEVEL:
            return TRIGGER_RESTART_LEVEL;
        }
    }
    
    return res;
}
