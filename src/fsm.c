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

    #define STATE_ASSERT( c ) \
    { \
        assert( (c) ); \
    } 
#endif

#define MAX_NESTED_STATES ( 3 )

_Static_assert( MAX_NESTED_STATES > 0U, "Max number of nested states must be greater than 0" );
_Static_assert( FIFO_BUFFER_SIZE > 0U, "Buffer size must be greater than zero" );
_Static_assert( ( FIFO_BUFFER_SIZE & ( FIFO_BUFFER_SIZE - 1U ) ) == 0U, "Buffer size must be power of two" );

typedef struct
{
    state_func_t * lca_out;
    state_func_t * lca_in;
}
lca_t;

static inline uint32_t TraverseToRoot( state_t * const source, state_func_t path[ static MAX_NESTED_STATES ] );
static void InitEventBuffer( state_fifo_t * const fsm_event );

static void InitEventBuffer( state_fifo_t * const fsm_event )
{
    STATE_ENTER_CRITICAL;
    fsm_event->read_index = 0U;
    fsm_event->write_index = 0U;
    fsm_event->fill = 0U;
    STATE_EXIT_CRITICAL;
}

extern void FSM_Init( state_t * state, state_fifo_t * fsm_event, state_ret_t (*initial_state) ( state_t * this, event_t s ) )
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
    state_ret_t ret = state->state( state, s );

    STATE_ASSERT( ret != RETURN_ENUM( Unhandled ) );

    while ( ret == RETURN_ENUM( Transition ) )
    {
        previous( state, event_Exit );
        previous = state->state;
        ret = state->state( state, event_Enter );
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

static lca_t DetermineLCA( uint32_t in_depth, state_func_t in_path[ static MAX_NESTED_STATES ], uint32_t out_depth, state_func_t out_path[ static MAX_NESTED_STATES ] )
{
    uint32_t min_depth = ( in_depth < out_depth ) ? in_depth : out_depth;
    min_depth++;
    lca_t lca;
    bool found_lca = false;

    uint32_t in_idx = in_depth;
    uint32_t out_idx = out_depth;

    lca.lca_in = &in_path[in_idx];
    lca.lca_out = &out_path[out_idx];
    for( uint32_t idx = 0U; idx < min_depth; idx++ )
    {
        if( in_path[in_idx] != out_path[out_idx] )
        {
            found_lca = true;
            break;
        }
        
        lca.lca_in = &in_path[in_idx];
        lca.lca_out = &out_path[out_idx];
        in_idx--;
        out_idx--;
    }

    /* If LCA not found, need to carry out a more thorough search for transition path */
    if( !found_lca )
    {
        /* 1. Is state trying to transition into itself? */
        if( in_path[0] == out_path[0] ) 
        {
            lca.lca_in = &in_path[1];
            lca.lca_out = &out_path[1];
            found_lca = true;
        }
        /* 2. Exit substate into parent state? */
        else if ( in_path[0] == out_path[1] )
        {
            lca.lca_in = &in_path[0];
            lca.lca_out = &out_path[1];
            found_lca = true;
        }
        else
        {
            /* Nowt */
        }
    }

    STATE_ASSERT( found_lca );
    return lca;
}

extern void FSM_HierarchicalDispatch( state_t * state, event_t s )
{
    STATE_ASSERT( state->state != NULL );
    STATE_ASSERT( state != NULL );
    STATE_ASSERT( s != _SIGNAL_ENUM( None ) );

    /* Always guaranteed to execute the first state */
    const state_func_t source = state->state;

    state_ret_t ret = state->state( state, s );

    while( ( ret == RETURN_ENUM( Unhandled ) ) && ( state->state != NULL ) )
    {
        ret = state->state( state, s );
    }

    if( ret == RETURN_ENUM( Transition ) )
    {
        /* Store the target state */
        const state_func_t target = state->state; 
    
        /* These hold the history up and down the state tree */
        state_func_t path_out[ MAX_NESTED_STATES ];
        state_func_t path_in[ MAX_NESTED_STATES ];

        const uint32_t in_depth = TraverseToRoot( state, path_in );
        state->state = source;
        const uint32_t out_depth = TraverseToRoot( state, path_out );

        lca_t lca = DetermineLCA( in_depth, path_in, out_depth, path_out );

        state->state = source;
        state_func_t * path = path_out;
        do
        {
            ret = (*path++)( state, EVENT( Exit ) );
            STATE_ASSERT( ret == RETURN_ENUM( Handled ) );
        }
        while( (*path) != *lca.lca_out );


        path = (lca.lca_in);
       
        while( (*path) != target )
        {
            (path--);
            ret = (*path)( state, EVENT( Enter ) );
            STATE_ASSERT( ret == RETURN_ENUM( Handled ) );
        }         
        /* Reassign original state */    
        state->state = target;
    
    }
    else
    {
        /* Restore original State */
        state->state = source;
    }

}

extern void FSM_FlushEvents( state_fifo_t * const fsm_event )
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

extern void FSM_AddEvent( state_fifo_t * const fsm_event, event_t s )
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

extern bool FSM_EventsAvailable( const state_fifo_t * const fsm_event )
{
    STATE_ASSERT( fsm_event != NULL );
    return ( fsm_event->fill > 0U );
}

extern event_t FSM_GetLatestEvent( state_fifo_t * const fsm_event )
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

