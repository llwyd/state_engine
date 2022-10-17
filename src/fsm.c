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
#elif TARGET_ESP32
    #define __ENTER_CRITICAL {  }
    #define __EXIT_CRITICAL  {  }
#else
    #define __ENTER_CRITICAL {  }
    #define __EXIT_CRITICAL  {  }
#endif

#define BUFFER_SIZE ( 32U )
#define MAX_NESTED_STATES ( 3 )

_Static_assert( MAX_NESTED_STATES > 0U );

struct fsm_events_t
{
    uint32_t read_index;
    uint32_t write_index;
    uint32_t fill;
    signal event[ BUFFER_SIZE ];
};

extern void FSM_Init( fsm_t * state, fsm_events_t * fsm_event )
{
    __ENTER_CRITICAL;
    fsm_event->read_index = 0U;
    fsm_event->write_index = 0U;
    fsm_event->fill = 0U;
    __EXIT_CRITICAL;

    FSM_Dispatch( state, signal_Enter );
}

extern void FSM_Dispatch( fsm_t * state, signal s )
{
    state_func_t previous = state->state;
    state_ret_t status = state->state( state, s );

    while ( status == RETURN_ENUM( Transition ) )
    {
        previous( state, signal_Exit );
        previous = state->state;
        status = state->state( state, signal_Enter );
    }
}

extern void FSM_HierarchicalDispatch( fsm_t * state, signal s )
{
    STATE_ASSERT( state->state != NULL );
    STATE_ASSERT( state != NULL );
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
            
            STATE_ASSERT( in_idx < MAX_NESTED_STATES ); 
            path_in[ in_idx ] = in;
            
            for( out_idx = 1; out_idx < out_max_nested; out_idx++ )
            {
                state->state = path_out[out_idx - 1]; 
                if( state->state != NULL )
                {
                    status = state->state( state, signal_None );
                    STATE_ASSERT( out_idx < MAX_NESTED_STATES ); 
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
        STATE_ASSERT( found_path );
        
        /* Exit nested states */
        for( uint32_t jdx = 0; jdx < out_idx; jdx++ )
        {
            state->state = path_out[jdx];
            status = state->state( state, signal_Exit );
            STATE_ASSERT( status == RETURN_ENUM( Handled ) );
        }

        /* Enter nested states */
        for( uint32_t idx = in_idx; idx > 0; idx-- )
        {
            STATE_ASSERT( idx != 0 );
            state->state = path_in[idx - 1];
            status = state->state( state, signal_Enter );
            STATE_ASSERT( status == RETURN_ENUM( Handled ) );
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


extern void FSM_FlushEvents( fsm_events_t * fsm_event )
{
    STATE_ASSERT( fsm_event != NULL );
    if( fsm_event->fill > 0U )
    {
        __ENTER_CRITICAL;
        fsm_event->read_index = fsm_event->write_index;
        fsm_event->fill = 0U;
        __EXIT_CRITICAL;
    }
}

extern void FSM_AddEvent( fsm_events_t * fsm_event, signal s )
{
    STATE_ASSERT( fsm_event != NULL );
    if( fsm_event->fill < BUFFER_SIZE )
    {
        __ENTER_CRITICAL;
        fsm_event->event[ fsm_event->write_index++ ] = s;
        fsm_event->fill++;
        fsm_event->write_index = ( fsm_event->write_index & ( BUFFER_SIZE - 1U ) );
        __EXIT_CRITICAL;
    }
}

extern bool FSM_EventsAvailable( fsm_events_t * fsm_event )
{
    STATE_ASSERT( fsm_event != NULL );
    return ( fsm_event->fill > 0U );
}

extern signal FSM_GetLatestEvent( fsm_events_t * fsm_event )
{
    signal s;
    STATE_ASSERT( fsm_event != NULL );

    __ENTER_CRITICAL;
    s = fsm_event->event[ fsm_event->read_index++ ];
    fsm_event->fill--;
    fsm_event->read_index = ( fsm_event->read_index & ( BUFFER_SIZE - 1U ) );
    __EXIT_CRITICAL;

    return s;
}

