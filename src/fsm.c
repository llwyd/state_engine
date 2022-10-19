/*
 *
 * FSM Engine
 *
 * T.L. 2022
 *
 */

#include "fsm.h"

#ifdef TARGET_ARM
    #define __ENTER_CRITICAL { asm("CPSID IF"); }
    #define __EXIT_CRITICAL  { asm("CPSIE IF"); }
#define _ASSERT( c ) \
    { \
        if ( !(c) ) \
        { \
            asm("CPSID IF"); \
            while(1); \
        } \
    } 

#elif TARGET_ESP32
    #define __ENTER_CRITICAL {  }
    #define __EXIT_CRITICAL  {  }
#else
    #include <stdio.h>
    #include <stdlib.h>
    #include <assert.h>
    
    #define __ENTER_CRITICAL {  }
    #define __EXIT_CRITICAL  {  }
    #define STATE_DISPATCH_START printf("[FSM]: Dispatching Event Start\n");
    #define STATE_DISPATCH_END printf("[FSM]: Dispatching Event End\n");

    #define STATE_TRAVERSE_START printf("[FSM]: Traversing States Start\n");
    #define STATE_TRAVERSE_END printf("[FSM]: Traversing States End\n");

    #define _ASSERT( c ) \
    { \
        assert( (c) ); \
    } 
#endif

#define BUFFER_SIZE ( 32U )
#define MAX_NESTED_STATES ( 3 )

_Static_assert( MAX_NESTED_STATES > 0U );
_Static_assert( BUFFER_SIZE > 0U );

struct fsm_events_t
{
    uint32_t read_index;
    uint32_t write_index;
    uint32_t fill;
    signal event[ BUFFER_SIZE ];
};

extern void FSM_Init( state_t * state, fsm_events_t * fsm_event )
{
    __ENTER_CRITICAL;
    fsm_event->read_index = 0U;
    fsm_event->write_index = 0U;
    fsm_event->fill = 0U;
    __EXIT_CRITICAL;

    FSM_Dispatch( state, signal_Enter );
}

extern void FSM_Dispatch( state_t * state, signal s )
{
    state_func_t previous = state->state;
    state_ret_t status = state->state( state, s );

    _ASSERT( status != RETURN_ENUM( Unhandled ) );

    while ( status == RETURN_ENUM( Transition ) )
    {
        previous( state, signal_Exit );
        previous = state->state;
        status = state->state( state, signal_Enter );
    }
}

extern void FSM_HierarchicalDispatch( state_t * state, signal s )
{
    _ASSERT( state->state != NULL );
    _ASSERT( state != NULL );
    _ASSERT( s != _SIGNAL_ENUM( None ) );
    STATE_DISPATCH_START;

    /* These hold the history up and down the state tree */
    state_func_t path_out[ MAX_NESTED_STATES ];
    state_func_t path_in[ MAX_NESTED_STATES ];

    /* Always guaranteed to execute the first state */
    path_out[0] = state->state; 
    const state_func_t source = state->state;

    state_ret_t status = state->state( state, s );

    while( ( status == RETURN_ENUM( Unhandled ) ) && ( state->state != NULL ) )
    {
        status = state->state( state, s );
    }

    if( status == RETURN_ENUM( Transition ) )
    {
        /* Perform Traversal algorithm */
        STATE_TRAVERSE_START;    

        /* Store the target state */
        path_in[0] = state->state;
        const state_func_t target = state->state; 

        /* Begin traversal by moving source and target up a super state */
        bool found_path = false;

        uint32_t in_max_nested = MAX_NESTED_STATES;
        uint32_t out_max_nested = MAX_NESTED_STATES;
        
        uint32_t in_idx = 0;
        uint32_t out_idx = 0;
        for( in_idx = 1; in_idx < in_max_nested; in_idx++ )
        {
            state_func_t in;
            state->state = path_in[ in_idx - 1 ];
            if( state->state != NULL )
            {
                status = state->state( state, signal_None );
            }
            in = state->state;
            
            _ASSERT( in_idx < MAX_NESTED_STATES ); 
            path_in[ in_idx ] = in;
            
            for( out_idx = 1; out_idx < out_max_nested; out_idx++ )
            {
                state->state = path_out[out_idx - 1]; 
                if( state->state != NULL )
                {
                    status = state->state( state, signal_None );
                    _ASSERT( out_idx < MAX_NESTED_STATES ); 
                    path_out[ out_idx ] = state->state;
                }
                else
                {
                    out_max_nested = out_idx;
                }

                /* if shared ancestor found, then break */
                if( in == state->state )
                {
                    found_path = true;

                    /* A legit use of GOTO! */
                    goto transition_path_found;
                }
               
            }
        }

transition_path_found:
        _ASSERT( found_path );
        
        /* Exit nested states */
        for( uint32_t jdx = 0; jdx < out_idx; jdx++ )
        {
            state->state = path_out[jdx];
            status = state->state( state, signal_Exit );
            _ASSERT( status == RETURN_ENUM( Handled ) );
        }

        /* Enter nested states */
        for( uint32_t idx = in_idx; idx > 0; idx-- )
        {
            _ASSERT( idx != 0 );
            state->state = path_in[idx - 1];
            status = state->state( state, signal_Enter );
            _ASSERT( status == RETURN_ENUM( Handled ) );
        }
        
        /* Reassign original state */    
        state->state = target;
    
        STATE_TRAVERSE_END;    
    }
    else
    {
        /* Restore original State */
        state->state = source;
    }

    STATE_DISPATCH_END;
}


extern void FSM_FlushEvents( fsm_events_t * const fsm_event )
{
    _ASSERT( fsm_event != NULL );
    if( fsm_event->fill > 0U )
    {
        __ENTER_CRITICAL;
        fsm_event->read_index = fsm_event->write_index;
        fsm_event->fill = 0U;
        __EXIT_CRITICAL;
    }
}

extern void FSM_AddEvent( fsm_events_t * const fsm_event, signal s )
{
    _ASSERT( fsm_event != NULL );
    if( fsm_event->fill < BUFFER_SIZE )
    {
        __ENTER_CRITICAL;
        fsm_event->event[ fsm_event->write_index++ ] = s;
        fsm_event->fill++;
        fsm_event->write_index = ( fsm_event->write_index & ( BUFFER_SIZE - 1U ) );
        __EXIT_CRITICAL;
    }
}

extern bool FSM_EventsAvailable( const fsm_events_t * const fsm_event )
{
    _ASSERT( fsm_event != NULL );
    return ( fsm_event->fill > 0U );
}

extern signal FSM_GetLatestEvent( fsm_events_t * const fsm_event )
{
    signal s;
    _ASSERT( fsm_event != NULL );

    __ENTER_CRITICAL;
    if( fsm_event->fill > 0U )
    {
        s = fsm_event->event[ fsm_event->read_index++ ];
        fsm_event->fill--;
        fsm_event->read_index = ( fsm_event->read_index & ( BUFFER_SIZE - 1U ) );
    }
    __EXIT_CRITICAL;

    return s;
}

