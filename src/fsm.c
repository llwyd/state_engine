/*
 *
 * FSM Engine
 *
 * T.L. 2022
 *
 */

#include "fsm.h"

#ifdef TARGET_ARM
    #define ENTER_CRITICAL { asm("CPSID IF"); }
    #define EXIT_CRITICAL  { asm("CPSIE IF"); }
#elif TARGET_ESP32
    #define ENTER_CRITICAL {  }
    #define EXIT_CRITICAL  {  }
#else
    #include <stdio.h>
    #include <stdlib.h>
    #include <assert.h>
    #define ENTER_CRITICAL {  }
    #define EXIT_CRITICAL  {  }
#endif

#define MAX_NESTED_STATES ( 3 )

struct fsm_events_t
{
    uint8_t read_index;
    uint8_t write_index;
    uint8_t fill;
    signal event[ BUFFER_SIZE ];
};

extern void FSM_Init( fsm_t * state, fsm_events_t * fsm_event )
{
    fsm_event->read_index = 0U;
    fsm_event->write_index = 0U;
    fsm_event->fill = 0U;

    FSM_Dispatch( state, signal_Enter );
}

extern fsm_status_t FSM_Transition( fsm_t * state, state_func f )
{
    assert( state->state != f );
    state->state = f;
    return fsm_Transition;
}

extern fsm_status_t FSM_SuperTransition( fsm_t * state, state_func f )
{
    assert( state->state != f );
    state->state = f;
    return fsm_SuperTransition;
}

extern void FSM_Dispatch( fsm_t * state, signal s )
{
    /* These hold the history up and down the state tree */
    state_func path_up[ MAX_NESTED_STATES ];
    state_func path_down[ MAX_NESTED_STATES ];

    /* Always guaranteed to execute the first state */
    uint32_t history_idx = 0U;
    path_up[history_idx] = state->state; 

    assert( state->state != NULL );
    fsm_status_t status = state->state( state, s );

    while( ( status == fsm_SuperTransition ) && ( state->state != NULL ) )
    {
        history_idx++;
        path_up[ history_idx ] = state->state;
        status = state->state( state, s );
    }

    if( status ==fsm_Transition )
    {
        /* Perform Traversal algorithm */
    }
    else
    {
        /* Restore original State */
        state->state = path_up[ 0U ];
    }


    /*
    state_func previous = state->state;
    fsm_status_t status = state->state( state, s );

    while ( status == fsm_Transition )
    {
        previous( state, signal_Exit );
        previous = state->state;
        status = state->state( state, signal_Enter );
    }
    */
}

extern void FSM_FlushEvents( fsm_events_t * fsm_event )
{
    if( fsm_event->fill > 0U )
    {
        ENTER_CRITICAL;
        fsm_event->read_index = fsm_event->write_index;
        fsm_event->fill = 0U;
        EXIT_CRITICAL;
    }
}

extern void FSM_AddEvent( fsm_events_t * fsm_event, signal s )
{
    if( fsm_event->fill < BUFFER_SIZE )
    {
        ENTER_CRITICAL;
        fsm_event->event[ fsm_event->write_index++ ] = s;
        fsm_event->fill++;
        fsm_event->write_index = ( fsm_event->write_index & ( BUFFER_SIZE - 1U ) );
        EXIT_CRITICAL;
    }
}

extern bool FSM_EventsAvailable( fsm_events_t * fsm_event )
{
    return ( fsm_event->fill > 0U );
}

extern signal FSM_GetLatestEvent( fsm_events_t * fsm_event )
{
    signal s;

    ENTER_CRITICAL;
    s = fsm_event->event[ fsm_event->read_index++ ];
    fsm_event->fill--;
    fsm_event->read_index = ( fsm_event->read_index & ( BUFFER_SIZE - 1U ) );
    EXIT_CRITICAL;

    return s;
}

