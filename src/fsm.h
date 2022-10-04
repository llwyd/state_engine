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


#ifdef TARGET_ARM
#define STATE_PRINT( SIGNAL )

#define STATE_ASSERT( c ) \
{ \
    if ( !(c) ) \
    { \
        asm("CPSID IF"); \
        while(1); \
    } \
} 

#elif TARGET_ESP32
#define STATE_PRINT( SIGNAL )
#else
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#define STATE_PRINT( SIGNAL ) printf("[%12s]->[%d]\n", __func__, SIGNAL )
#define STATE_DISPATCH_START printf("[FSM]: Dispatching Event Start\n");
#define STATE_DISPATCH_END printf("[FSM]: Dispatching Event End\n");

#define STATE_TRAVERSE_START printf("[FSM]: Traversing States Start\n");
#define STATE_TRAVERSE_END printf("[FSM]: Traversing States End\n");

#define STATE_ASSERT( c ) \
{ \
    assert( (c) ); \
} 

#endif

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

extern void FSM_Init( fsm_t * state, fsm_events_t * fsm_event );

/* Event Dispatchers */
extern void FSM_HierarchicalDispatch( fsm_t * state, signal s );
extern void FSM_Dispatch( fsm_t * state, signal s );

extern fsm_status_t FSM_Transition( fsm_t * state, state_func f );
extern fsm_status_t FSM_SuperTransition( fsm_t * state, state_func f );

/* Event queuing */
extern void FSM_FlushEvents( fsm_events_t * fsm_event );
extern void FSM_AddEvent( fsm_events_t * fsm_event, signal s);
extern signal FSM_GetLatestEvent( fsm_events_t * fsm_event );
extern bool FSM_EventsAvailable( fsm_events_t * fsm_event );

#endif /* _FSM_H_ */

