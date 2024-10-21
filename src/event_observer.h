#ifndef EVENT_OBS_H_
#define EVENT_OBS_H_

#include "state.h"

#define MAX_SUBSCRIBERS (4U)

typedef struct
{
    state_t * subscribers[MAX_SUBSCRIBERS];
    uint32_t subscriptions;
}
event_observer_t;

#define EVENT_OBS_ARRAY(x) [SIGNAL_ENUM_(x)] = {.subscribers = {NULL}, .subscriptions = 0U},

#define GENERATE_EVENT_OBSERVERS(EV) \
    event_observer_t observer [] = \
    { \
        EV(EVENT_OBS_ARRAY) \
    }

#endif /* EVENT_OBS_H */
