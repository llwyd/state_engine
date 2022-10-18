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

/* Global Settings */
#define STATE_ASSERT_ENABLE == false
#define STATE_DEBUG_ENABLE == false

/* Core State Machine Defines and helper macros */
#define DEFAULT_SIGNALS(SIGNAL) \
    SIGNAL( None ) \
    SIGNAL( Enter ) \
    SIGNAL( Exit ) \

#define STATE_RETURN_CODES \
    RETURN( None ) \
    RETURN( Handled ) \
    RETURN( Unhandled ) \
    RETURN( Transition ) \

#define SIGNAL_ENUM(x) signal_##x,
#define _SIGNAL_ENUM(x) signal_##x
#define RETURN_ENUM(x) return_##x
#define EVENT(x) _SIGNAL_ENUM(x)
#define STATE(x) State_##x

#define _SIGNAL_STR(x) #x
#define _SIGNAL_LOOKUP(x) [_SIGNAL_ENUM(x)] = _SIGNAL_STR(x),


#define GENERATE_SIGNALS( SIG ) \
    enum Signal \
    { \
        _SIGNAL_ENUM( Tick ) = SIGNAL_ENUM( DefaultCount ) \
        SIG( SIGNAL_ENUM ) \
    }

#define GENERATE_SIGNAL_STRINGS( SIG ) \
    static const char *_signal_str[] = \
    { \
        DEFAULT_SIGNALS( _SIGNAL_LOOKUP ) \
        _SIGNAL_LOOKUP( Tick ) \
        SIG( _SIGNAL_LOOKUP ) \
    } \

#define STATE_DEBUG( x ) printf("%s -> %s Signal\n", __func__, _signal_str[x] )


#define PARENT( _state, parent_state ) _state->state = STATE(parent_state);  ret = RETURN_ENUM( Unhandled )
#define TRANSITION( _state, new_state ) _state->state = STATE(new_state);  ret = RETURN_ENUM( Transition )
#define HANDLED( _state ) ret = RETURN_ENUM ( Handled )
#define NO_PARENT( _state ) _state->state = NULL; ret = RETURN_ENUM( Unhandled )


/* Platform Specific Stuff */

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
#error "Esp32 target not yet supported"

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
    DEFAULT_SIGNALS(SIGNAL_ENUM)
    SIGNAL_ENUM(DefaultCount)
};

typedef enum
{
    #define RETURN(x) RETURN_ENUM(x),
        STATE_RETURN_CODES
    #undef RETURN
}
state_ret_t;

/* Circular buffer for FSM events. This is declared inside the .c file */
typedef struct fsm_events_t fsm_events_t;

/* Forward declaration so that function pointer with state can return itself */
typedef struct fsm_t fsm_t;

/* Function pointer that holds the state to execute */
typedef state_ret_t ( *state_func_t ) ( fsm_t * this, signal s );

struct fsm_t
{
    state_func_t state;
};

extern void FSM_Init( fsm_t * state, fsm_events_t * fsm_event );

/* Event Dispatchers */
extern void FSM_HierarchicalDispatch( fsm_t * state, signal s );
extern void FSM_Dispatch( fsm_t * state, signal s );

extern state_ret_t FSM_Transition( fsm_t * state, state_func_t f );
extern state_ret_t FSM_SuperTransition( fsm_t * state, state_func_t f );

/* Event queuing */
extern void FSM_FlushEvents( fsm_events_t * fsm_event );
extern void FSM_AddEvent( fsm_events_t * fsm_event, signal s);
extern signal FSM_GetLatestEvent( fsm_events_t * fsm_event );
extern bool FSM_EventsAvailable( fsm_events_t * fsm_event );

#endif /* _FSM_H_ */

