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

_Static_assert( MAX_NESTED_STATES > 0U );

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
    state_func path_out[ MAX_NESTED_STATES ];
    state_func path_in[ MAX_NESTED_STATES ];

    /* Always guaranteed to execute the first state */
    path_out[0] = state->state; 
    const state_func source = state->state;

    assert( state->state != NULL );
    fsm_status_t status = state->state( state, s );

    while( ( status == fsm_SuperTransition ) && ( state->state != NULL ) )
    {
        status = state->state( state, s );
    }

    if( status == fsm_Transition )
    {
        /* Perform Traversal algorithm */
        printf("--- Traversing states ---\n"); 
        /* Store the target state */
        path_in[0] = state->state;
        const state_func target = state->state;
       

        /* Begin traversal by moving source and target up a super state */
        bool found_path = false;

        uint32_t in_max_nested = MAX_NESTED_STATES;
        uint32_t out_max_nested = MAX_NESTED_STATES;
        
        int i = 0;
        int j = 0;
        for( i = 0; i < in_max_nested; i++ )
        {
            state_func in;
            in = path_in[i];
            state->state = in;
            if( in != NULL )
            {
                status = in( state, signal_Traverse );
            }
            in = state->state;
            
            for( j = 0; j < out_max_nested; j++ )
            {
                state_func out = path_out[j]; 
                state->state = out;
                if( out != NULL )
                {
                    status = out( state, signal_Traverse );
                }
                out = state->state;
                
                path_out[ j + 1 ] = out;

                /* if shared ancestor found, then break */
                if( in == out )
                {
                    printf("Ancestor Found\n");
                    found_path = true;

                    /* A legit use of GOTO! */
                    goto transition;
                }
            }
            path_in[ i + 1 ] = in;
        }

transition:
        assert( found_path );

        i++;
        j++;

        /* Exit nested states */
        for( int jdx = 0; jdx < j; jdx++ )
        {
            state->state = path_out[jdx];
            status = state->state( state, signal_Exit );
            assert( status == fsm_Handled );
        }

        /* Enter nested states */
        for( int idx = i; idx > 0; idx-- )
        {
            state->state = path_in[idx - 1];
            status = state->state( state, signal_Enter );
            assert( status == fsm_Handled );
        }
        
        state->state = target;
        
        printf("--- Traversal complete ---\n"); 
    }
    else
    {
        /* Restore original State */
        state->state = source;
    }
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

