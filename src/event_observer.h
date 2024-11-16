#ifndef EVENT_OBS_H_
#define EVENT_OBS_H_

#include "state.h"
#include <assert.h>
#include <stdio.h>

#define MAX_SUBSCRIPTIONS (4U)

typedef struct
{
    state_t * subscriber[MAX_SUBSCRIPTIONS];
    uint32_t subscriptions;
}
event_observer_t;

#define EVENT_OBS_ARRAY(x) [EVENT_ENUM_(x)] = {.subscriber = {NULL}, .subscriptions = 0U},

#define GENERATE_EVENT_OBSERVERS(NAME, EV) \
    event_observer_t NAME [] = \
    { \
        EV(EVENT_OBS_ARRAY) \
    }

extern void EventObserver_Init(event_observer_t * const obs, uint32_t num_events);
extern void EventObserver_Subscribe(event_observer_t * const obs, event_t event, state_t * subscriber);
extern const event_observer_t * const EventObserver_GetSubs(event_observer_t * const obs, event_t e);

#endif /* EVENT_OBS_H */
