/*
 *
 * State Machine Engine
 *
 * T.L. 2022
 *
 */

#include "state.h"

#ifdef TARGET_ARM
    #define STATE_ENTER_CRITICAL { asm("CPSID IF"); }
    #define STATE_EXIT_CRITICAL  { asm("CPSIE IF"); }
    #define STATE_ASSERT( c ) \
        { \
            if ( !(c) ) \
            { \
                asm("CPSID IF"); \
                while(1) \
                { \
                } \
            } \
        } 

#elif TARGET_ESP32
    #include <stdio.h>
    #include <stdlib.h>
    #include <assert.h>
    
    #define STATE_ENTER_CRITICAL {  }
    #define STATE_EXIT_CRITICAL  {  }
    
    #define STATE_ASSERT( c ) \
    { \
        assert( (c) ); \
    } 
#else
//cppcheck-suppress misra-c2012-21.6
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


_Static_assert( MAX_NESTED_STATES > 0U, "Max number of nested states must be greater than 0" );

typedef struct
{
    uint32_t out;
    uint32_t in;
}
lca_t;

typedef struct
{
    state_t state; 
    state_func_t * path_in;
    state_func_t * path_out;
    uint32_t in_depth;
    uint32_t out_depth;
    state_func_t target;
    state_func_t source;
    lca_t lca;
}
transition_t;

/*Handling a HSM transition requires an FSM */
static state_ret_t State_TransitionStart( state_t * this, event_t s );
static state_ret_t State_TransitionExiting( state_t * this, event_t s );
static state_ret_t State_TransitionEntering( state_t * this, event_t s );

static inline uint32_t TraverseToRoot( state_t * const source, state_func_t path[ MAX_NESTED_STATES ] );

static lca_t DetermineLCA( uint32_t in_depth, 
        state_func_t in_path[ MAX_NESTED_STATES ], 
        uint32_t out_depth,
        state_func_t out_path[ MAX_NESTED_STATES ] );

/* These macros are for recording history of state executions, transitions etc for unit testing */
#ifdef UNIT_TESTS
    static history_fifo_t state_history;

    extern void STATE_UnitTestInit( void );
    #define STATE_EXECUTE( current_state, current_event ) \
    { \
        state_history_data_t hist = \
        { \
            .state=(current_state)->state, \
            .event=(current_event), \
        }; \
        FIFO_Enqueue(&state_history, hist);\
        ret =  (current_state)->state( (current_state), (current_event) ); \
    }
#else
    #define STATE_EXECUTE( current_state, current_event ) ret = (current_state)->state( (current_state), (current_event) )
#endif

extern void STATEMACHINE_Init( state_t * state,  state_ret_t (*initial_state) ( state_t * this, event_t s ) )
{
    STATE_ASSERT( state != NULL );
    STATE_ASSERT( initial_state != NULL );

    state_func_t init_path[ MAX_NESTED_STATES ];
    state->state = initial_state;
    state_ret_t ret;

    uint32_t idx = TraverseToRoot( state, init_path );

    /* This assertion failing implies an initial state of NULL */
    STATE_ASSERT( idx > 0U );
    STATE_ASSERT( idx < MAX_NESTED_STATES );

    for( ; idx > 0U; idx-- )
    {
        STATE_ASSERT( idx > 0U );
        state->state = *init_path[ idx - 1U ];
        STATE_EXECUTE( state, EVENT( Enter ) );
        STATE_ASSERT( ret == RETURN( Handled ) );
    }

    STATE_ASSERT( state->state == initial_state );
}

/* A 'simple' dispatch for a flat state machine */
extern void STATEMACHINE_FlatDispatch( state_t * state, event_t s )
{
    state_func_t previous = state->state;
    state_ret_t ret = state->state( state, s );

    STATE_ASSERT( ret != RETURN( Unhandled ) );

    while ( ret == RETURN( Transition ) )
    {
        previous( state, event_Exit );
        previous = state->state;
        ret = state->state( state, event_Enter );
    }
}

static inline uint32_t TraverseToRoot( state_t * const source, state_func_t path[ MAX_NESTED_STATES ] )
{
    STATE_ASSERT( source != NULL );
    STATE_ASSERT( path != NULL );

    state_ret_t ret;
    uint32_t path_length = 0U;
    
    for( path_length = 0U; path_length < MAX_NESTED_STATES; path_length++ )
    {
        path[ path_length ] = *source->state;
        if( source->state == NULL )
        {
            /* Root of the HSM has been reached */
            break;
        }

        STATE_ASSERT( source->state != NULL );
        ret = source->state( source, EVENT( None ) );
        STATE_ASSERT( ret == RETURN( Unhandled ) );
    }

    return path_length;
}

static lca_t DetermineLCA( uint32_t in_depth, 
        state_func_t in_path[ MAX_NESTED_STATES ], 
        uint32_t out_depth, 
        state_func_t out_path[ MAX_NESTED_STATES ] )
{
    uint32_t min_depth = ( in_depth < out_depth ) ? in_depth : out_depth;
    min_depth++;
    lca_t lca;
    bool found_lca = false;

    uint32_t in_idx = in_depth;
    uint32_t out_idx = out_depth;

    lca.in = in_idx;
    lca.out = out_idx;
    for( uint32_t idx = 0U; idx < min_depth; idx++ )
    {
        if( in_path[in_idx] != out_path[out_idx] )
        {
            found_lca = true;
            break;
        }
        
        lca.in = in_idx;
        lca.out = out_idx;
        in_idx--;
        out_idx--;
    }

    /* If LCA not found, need to carry out a more thorough search for transition path */
    if( !found_lca )
    {
        /* 1. Is state trying to transition into itself? */
        if( in_path[0] == out_path[0] ) 
        {
            lca.in = 1u;
            lca.out = 1u;
            found_lca = true;
        }
        /* 2. Exit substate into parent state? */
        else if ( in_path[0] == out_path[1] )
        {
            lca.in = 0u;
            lca.out = 1u;
            found_lca = true;
        }
        /* 3. All out states are NULL, which will happen if transition upon exit */
        else if( out_path[0] == NULL )
        {
            lca.in = in_depth;
            lca.out = 0;
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

static state_ret_t State_TransitionStart( state_t * this, event_t s )
{
    state_ret_t ret;
    ret = HANDLED(this);
    transition_t * transition = ( transition_t * ) this;

    switch( s )
    {
        case EVENT( Enter ):
        {
            for( uint32_t idx = 0; idx < MAX_NESTED_STATES; idx++ )
            {
                transition->path_in[idx] = NULL;
                transition->path_out[idx] = NULL;
            }
            /* Determine paths to root */
            transition->state.state = *transition->target;
            transition->in_depth = TraverseToRoot( &transition->state, transition->path_in );
            transition->state.state = *transition->source;
            transition->out_depth = TraverseToRoot( &transition->state, transition->path_out );
            /* Find common ancestor */
            transition->lca = DetermineLCA( transition->in_depth, transition->path_in, transition->out_depth, transition->path_out );
            /* Begin exiting */
            ret = TRANSITION( this, TransitionExiting );
        }
            break;
        case EVENT( None ):
            STATE_ASSERT( false );
            break;
        default:
            break;
    }
    return ret;
}

static state_ret_t State_TransitionEntering( state_t * this, event_t s )
{
    state_ret_t ret;
    ret = HANDLED(this);
    transition_t * transition = ( transition_t * ) this;

    switch( s )
    {
        case EVENT( Enter ):
        {
            uint32_t jdx = transition->lca.in - 1U;
            ret = HANDLED(this);
            for( uint32_t idx = 0;  idx < ( transition->lca.in - 0U ); idx++ )
            {
                transition->state.state = *transition->path_in[jdx];
                jdx--;
                STATE_EXECUTE( &transition->state, EVENT( Enter ) );
                STATE_ASSERT( ret != RETURN( Unhandled ) );
                if( ret == RETURN( Transition ) )
                {
                    transition->source = *transition->path_in[jdx + 1U];
                    transition->target = transition->state.state;
                    ret = TRANSITION(this, TransitionStart );
                    break;
                }
                else
                {
                    ret = HANDLED(this);
                }
            }

        }
            break;
        case EVENT( None ):
            STATE_ASSERT( false );
            break;
        default:
            break;
    }
    return ret;
}


static state_ret_t State_TransitionExiting( state_t * this, event_t s )
{
    state_ret_t ret;
    ret = HANDLED(this);
    transition_t * transition = ( transition_t * ) this;

    switch( s )
    {
        case EVENT( Enter ):
        {
            ret = TRANSITION(this, TransitionEntering );
            for( uint32_t idx = 0; idx < transition->lca.out; idx++ )
            {
                transition->state.state = *transition->path_out[idx];
                STATE_EXECUTE( &transition->state, EVENT( Exit ) );
                STATE_ASSERT( ret != RETURN( Unhandled ) );
                if( ret == RETURN( Transition ) )
                {
                    uint32_t next_state = idx + 1;
                    if( next_state < MAX_NESTED_STATES )
                    {
                        transition->source = *transition->path_out[next_state];
                    }
                    else
                    {
                        transition->source = NULL;
                    }
                    
                    transition->target = transition->state.state;
                    ret = TRANSITION(this, TransitionStart );
                    break;
                }
                else
                {
                    ret = TRANSITION(this, TransitionEntering );
                }
            } 
        }
            break;
        case EVENT( None ):
            STATE_ASSERT( false );
            break;
        default:
            break;
    }
    return ret;
}

extern void STATEMACHINE_Dispatch( state_t * state, event_t s )
{
    STATE_ASSERT( state->state != NULL );
    STATE_ASSERT( state != NULL );
    STATE_ASSERT( s != (event_t)EVENT( None ) );

    /* Always guaranteed to execute the first state */
    state_func_t source = state->state;

    state_ret_t ret;
    
    STATE_EXECUTE( state, s );

    while( ( ret == RETURN( Unhandled ) ) && ( state->state != NULL ) )
    {
        STATE_EXECUTE( state, s );
    }

    if( ret == RETURN( Transition ) )
    {
        /* Store the target state */
        state_func_t target = state->state; 
    
        /* These hold the history up and down the state tree */
        state_func_t path_out[ MAX_NESTED_STATES ];
        state_func_t path_in[ MAX_NESTED_STATES ];

        /* FSM within HSM to handle transitions */
        transition_t transition =
        {
            .state = *state,
            .path_out = path_out,
            .path_in = path_in,
            .in_depth = 0U,
            .out_depth = 0U,
            .source = source,
            .target = target,
        };
    
        /* Dogfooding to handle transition */
        transition.state.state = STATE( TransitionStart );
        STATEMACHINE_FlatDispatch( &(transition.state), EVENT( Enter ) );

        /* Reassign original state */    
        state->state = transition.target;
    
    }
    else
    {
        /* Restore original State */
        state->state = source;
    }

}

#ifdef UNIT_TESTS
extern void STATE_UnitTestInit( void )
{
    History_Init( &state_history );
}

extern fifo_base_t * STATE_GetHistory( void )
{
    return &state_history.base;
}

#endif


