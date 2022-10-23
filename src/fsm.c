/*
 *
 * FSM Engine
 *
 * T.L. 2022
 *
 */

#include "fsm.h"

#ifdef TARGET_ARM
    #define STATE_ENTER_CRITICAL { asm("CPSID IF"); }
    #define STATE_EXIT_CRITICAL  { asm("CPSIE IF"); }
#define STATE_ASSERT( c ) \
    { \
        if ( !(c) ) \
        { \
            asm("CPSID IF"); \
            while(1); \
        } \
    } 

#elif TARGET_ESP32
    #define STATE_ENTER_CRITICAL {  }
    #define STATE_EXIT_CRITICAL  {  }
#else
    #include <stdio.h>
    #include <stdlib.h>
    #include <assert.h>
    
    #define STATE_ENTER_CRITICAL {  }
    #define STATE_EXIT_CRITICAL  {  }
    #define STATE_DISPATCH_START printf("[FSM]: Dispatching Event Start\n");
    #define STATE_DISPATCH_END printf("[FSM]: Dispatching Event End\n");

    #define STATE_TRAVERSE_START printf("[FSM]: Traversing States Start\n");
    #define STATE_TRAVERSE_END printf("[FSM]: Traversing States End\n");

    #define STATE_ASSERT( c ) \
    { \
        assert( (c) ); \
    } 
#endif

#define MAX_NESTED_STATES ( 3 )

_Static_assert( MAX_NESTED_STATES > 0U, "Max number of nested states must be greater than 0" );
_Static_assert( FIFO_BUFFER_SIZE > 0U, "Buffer size must be greater than zero" );
_Static_assert( ( FIFO_BUFFER_SIZE & ( FIFO_BUFFER_SIZE - 1U ) ) == 0U, "Buffer size must be power of two" );


static inline uint32_t TraverseToRoot( state_t * const source, state_func_t path[ static MAX_NESTED_STATES ] );
static void InitEventBuffer( fsm_events_t * const fsm_event );

static void InitEventBuffer( fsm_events_t * const fsm_event )
{
    STATE_ENTER_CRITICAL;
    fsm_event->read_index = 0U;
    fsm_event->write_index = 0U;
    fsm_event->fill = 0U;
    STATE_EXIT_CRITICAL;
}

extern void FSM_Init( state_t * state, fsm_events_t * fsm_event, state_ret_t (*initial_state) ( state_t * this, event_t s ) )
{
    STATE_ASSERT( state != NULL );
    STATE_ASSERT( fsm_event != NULL );
    STATE_ASSERT( initial_state != NULL );

    state_func_t init_path[ MAX_NESTED_STATES ];
    InitEventBuffer( fsm_event );
    
    state->state = initial_state;
    state_ret_t ret;

    uint32_t idx = TraverseToRoot( state, init_path );

    /* This assertion failing implies an initial state of NULL */
    STATE_ASSERT( idx > 0U );
    STATE_ASSERT( idx < MAX_NESTED_STATES );

    for( ; idx > 0U; idx-- )
    {
        STATE_ASSERT( idx > 0U );
        state->state = init_path[ idx - 1U ];
        ret = state->state( state, EVENT( Enter ) );
        STATE_ASSERT( ret == RETURN_ENUM( Handled ) );
    }

    STATE_ASSERT( state->state == initial_state );
}

extern void FSM_Dispatch( state_t * state, event_t s )
{
    state_func_t previous = state->state;
    state_ret_t status = state->state( state, s );

    STATE_ASSERT( status != RETURN_ENUM( Unhandled ) );

    while ( status == RETURN_ENUM( Transition ) )
    {
        previous( state, event_Exit );
        previous = state->state;
        status = state->state( state, event_Enter );
    }
}

static inline uint32_t TraverseToRoot( state_t * const source, state_func_t path[ static MAX_NESTED_STATES ] )
{
    STATE_ASSERT( source != NULL );
    STATE_ASSERT( source->state != NULL );
    STATE_ASSERT( path != NULL );

    state_ret_t ret;
    uint32_t path_length = 0U;
    
    for( path_length = 0U; path_length < MAX_NESTED_STATES; path_length++ )
    {
        path[ path_length ] = source->state;
        if( source->state == NULL )
        {
            /* Root of the HSM has been reached */
            break;
        }

        STATE_ASSERT( source->state != NULL );
        ret = source->state( source, EVENT( None ) );
        STATE_ASSERT( ret == RETURN_ENUM( Unhandled ) );
    }

    return path_length;
}

extern void FSM_HierarchicalDispatch( state_t * state, event_t s )
{
    STATE_ASSERT( state->state != NULL );
    STATE_ASSERT( state != NULL );
    STATE_ASSERT( s != _SIGNAL_ENUM( None ) );
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
                status = state->state( state, event_None );
            }
            in = state->state;
            
            STATE_ASSERT( in_idx < MAX_NESTED_STATES ); 
            path_in[ in_idx ] = in;
            
            for( out_idx = 1; out_idx < out_max_nested; out_idx++ )
            {
                state->state = path_out[out_idx - 1]; 
                if( state->state != NULL )
                {
                    status = state->state( state, event_None );
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
            status = state->state( state, EVENT( Exit ) );
            STATE_ASSERT( status == RETURN_ENUM( Handled ) );
        }

        /* Enter nested states */
        for( uint32_t idx = in_idx; idx > 0; idx-- )
        {
            STATE_ASSERT( idx != 0 );
            state->state = path_in[idx - 1];
            status = state->state( state, EVENT( Enter ) );
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


extern void FSM_FlushEvents( fsm_events_t * const fsm_event )
{
    STATE_ASSERT( fsm_event != NULL );
    if( fsm_event->fill > 0U )
    {
        STATE_ENTER_CRITICAL;
        fsm_event->read_index = fsm_event->write_index;
        fsm_event->fill = 0U;
        STATE_EXIT_CRITICAL;
    }
}

extern void FSM_AddEvent( fsm_events_t * const fsm_event, event_t s )
{
    STATE_ASSERT( fsm_event != NULL );
    if( fsm_event->fill < FIFO_BUFFER_SIZE )
    {
        STATE_ENTER_CRITICAL;
        fsm_event->event[ fsm_event->write_index++ ] = s;
        fsm_event->fill++;
        fsm_event->write_index = ( fsm_event->write_index & ( FIFO_BUFFER_SIZE - 1U ) );
        STATE_EXIT_CRITICAL;
    }
}

extern bool FSM_EventsAvailable( const fsm_events_t * const fsm_event )
{
    STATE_ASSERT( fsm_event != NULL );
    return ( fsm_event->fill > 0U );
}

extern event_t FSM_GetLatestEvent( fsm_events_t * const fsm_event )
{
    event_t s;
    STATE_ASSERT( fsm_event != NULL );

    STATE_ENTER_CRITICAL;
    if( fsm_event->fill > 0U )
    {
        s = fsm_event->event[ fsm_event->read_index++ ];
        fsm_event->fill--;
        fsm_event->read_index = ( fsm_event->read_index & ( FIFO_BUFFER_SIZE - 1U ) );
    }
    STATE_EXIT_CRITICAL;

    return s;
}

