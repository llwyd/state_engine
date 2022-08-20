/*
 *
 * FSM Engine
 *
 * T.L. 2022
 *
 */

#ifndef _FSM_H_
#define _FSM_H_

#include <stdint.h>
#include <stdbool.h>

#define BUFFER_SIZE ( 32U )

/* Signal to send events to a given state */
typedef uint32_t signal;

/* Default signals for state machine */
enum DefaultSignals
{
    signal_None,
    signal_Enter,
    signal_Exit,
    signal_Traverse, /* Used to return a state's super state * */

    signal_Count,
};

/* Circular buffer for FSM events */
typedef struct fsm_events_t fsm_events_t;

typedef enum
{
    fsm_Handled,
    fsm_Transition,
    fsm_SuperTransition,
    fsm_Ignored,
} fsm_status_t;

/* Forward declaration so that function pointer with state can return itself */
typedef struct fsm_t fsm_t;

/* Function pointer that holds the state to execute */
typedef fsm_status_t ( *state_func ) ( fsm_t * this, signal s );

struct fsm_t
{
    state_func state;
};

typedef struct hsm_t hsm_t;

struct hsm_t
{
    state_func state;
    hsm_t * super_state;
};

extern void FSM_Init( fsm_t * state, fsm_events_t * fsm_event );

/* Event Dispatcher */
extern void FSM_Dispatch( fsm_t * state, signal s );

/* Event queuing */
extern void FSM_FlushEvents( fsm_events_t * fsm_event );
extern void FSM_AddEvent( fsm_events_t * fsm_event, signal s);
extern signal FSM_GetLatestEvent( fsm_events_t * fsm_event );
extern bool FSM_EventsAvailable( fsm_events_t * fsm_event );

#endif /* _FSM_H_ */

