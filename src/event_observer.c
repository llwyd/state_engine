#include "event_observer.h"

extern void EventObserver_Init(event_observer_t * const obs, uint32_t num_events)
{
    assert(obs != NULL);
    assert(num_events > 0U );

    for(uint32_t idx = 0U; idx < num_events; idx++)
    {
        event_observer_t * observer = &obs[idx];
        for(uint32_t jdx = 0U; jdx < MAX_SUBSCRIPTIONS; jdx++)
        {
            observer->subscriber[jdx] = 0U;
        }
        observer->subscriptions = 0U;
    }
}

extern void EventObserver_Subscribe(event_observer_t * const obs, event_t event, state_t * subscriber)
{
    assert( obs != NULL );
    assert( subscriber != NULL );

    const uint32_t idx = (uint32_t)event;
    event_observer_t * const observer = &obs[idx];
    
    assert( observer->subscriptions <= MAX_SUBSCRIPTIONS );
    /* Ensure it hasn't already been subscribed */
    for(uint32_t kdx = 0; kdx < observer->subscriptions; kdx++)
    {
        assert( observer->subscriber[kdx] != subscriber );
    }

    const uint32_t jdx = observer->subscriptions;
    observer->subscriber[jdx] = subscriber;
    observer->subscriptions++;
}

