#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

typedef uint32_t signal_t;

typedef enum
{
    fsm_None,
    fsm_Handled,
    fsm_Unhandled, /* If unhandled, need to traverse up the states */
    fsm_Transition,
} state_return_t;

#define STATES \
    X( A, ROOT ) \
    X( B, ROOT ) \
    X( A0, A ) \
    X( A1, A ) \
    X( B0, B ) \
    X( A00, A0 ) \

#define SIGNALS \
    X( Entry ) \
    X( Exit ) \
    X( Tick ) \
    X( TransitionToA ) \
    X( TransitionToB ) \
    X( TransitionToA0 ) \
    X( TransitionToA1 ) \
    X( TransitionToB0 )

#define ENUM_(x) STATE_##x
#define FUNC_(x) State##x
#define SIGNAL_ENUM(x) signal_##x



typedef enum
{
    STATE_ROOT = 0U,
    #define X(a,b) ENUM_(a),
        STATES
    #undef X
    STATE_COUNT
}
state_t;

enum Signals
{
    #define X(a) SIGNAL_ENUM(a),
        SIGNALS
    #undef X
    signal_Count,
};

const char *signal_str[] = 
{
    #define X(a) [SIGNAL_ENUM(a)] = #a,
        SIGNALS
    #undef X
};

const char *state_str[] =
{
    [STATE_ROOT] = "ROOT",
    #define X(a,b) [ENUM_(a)] = #a,
        STATES
    #undef X
};

#define FUNC_RETURN  state_return_t 
#define FUNC_ARGS ( state_t * state, signal_t s )

#define PRINT_SIGNAL( x ) printf("%s -> %s Signal\n", __func__, signal_str[x] )

#define X(a,b) FUNC_RETURN FUNC_(a) FUNC_ARGS;
    STATES
#undef X

const state_t parent_lookup[ ] =
{
    #define X(a, b) [ ENUM_(a) ] = ENUM_(b),
        STATES
    #undef X
};

FUNC_RETURN (*func_lookup[ ]) FUNC_ARGS =
{
    [ STATE_ROOT ] = NULL,
    #define X(a, b) [ ENUM_(a) ] = FUNC_(a),
        STATES
    #undef X
};

FUNC_RETURN StateB0 FUNC_ARGS
{
    PRINT_SIGNAL(s);
    state_return_t ret = fsm_Handled;
    return ret;
}

FUNC_RETURN StateA1 FUNC_ARGS
{
    PRINT_SIGNAL(s);
    state_return_t ret = fsm_Handled;
    return ret;
}

FUNC_RETURN StateA0 FUNC_ARGS
{
    PRINT_SIGNAL(s);
    state_return_t ret = fsm_Unhandled;

    switch( s )
    {
        case signal_TransitionToB:
            *state = STATE_A1;
            ret = fsm_Transition;
            break;
        case signal_Entry:
        case signal_Exit:
            ret = fsm_Handled;
            break;
        default:
            break;
    }

    return ret;
}

FUNC_RETURN StateA FUNC_ARGS
{
    PRINT_SIGNAL(s);
    state_return_t ret = fsm_Unhandled;

    switch( s )
    {
        case signal_Entry:
        case signal_Exit:
            ret = fsm_Handled;
            break;
        default:
            break;
    }

    return ret;
}

FUNC_RETURN StateB FUNC_ARGS
{
    printf("Func: %s\n", __func__);
    state_return_t ret = fsm_Handled;
    return ret;
}

FUNC_RETURN StateA00 FUNC_ARGS
{
    printf("Func: %s\n", __func__);
    state_return_t ret = fsm_Handled;
    return ret;
}

void Dispatch( state_t * state, signal_t s )
{
    state_t previous = *state;
    state_return_t status = func_lookup[ *state ]( state, s );

    while( (status == fsm_Unhandled ) && ( *state != STATE_ROOT ) )
    {
        *state = parent_lookup[ *state ];
        if( *state != STATE_ROOT )
        {
            status = func_lookup[ *state ]( state, s );
        }
    }

    if( status == fsm_Transition )
    {
        state_t path[ 3U ];
        printf("Begin Transition\n");
        state_t target = *state;
        
        state_t source_parent = previous;
        state_t target_parent = target;
        printf("[%s] <-> [%s]\n", state_str[target_parent], state_str[source_parent]);
        
        for( int source_idx = 1; source_idx < 3U; source_idx++ )
        {
            source_parent = parent_lookup[source_parent];
            for( int target_idx = 1; target_idx < 3U; target_idx++ )
            {
                target_parent = parent_lookup[target_parent];
                if( target_parent == source_parent )
                {
                    printf("Shared Ancestor Found\n");
                    printf("[%s] <-> [%s]\n", state_str[target_parent], state_str[source_parent]);
                    goto transition_found;
                }
            }
        }
        
transition_found:
    }
    else
    {
        *state = previous;
    }
}

void DetermineStackSize( void )
{
    printf("Determining Stack Size\n");

    printf("State count: %d\n", (int)STATE_COUNT);

    int nest_size = 0U;
    
    int j = STATE_COUNT - 1U;
    for( int i = 1; i < STATE_COUNT; i++ )
    {
        int current_nest = 1;
        assert( j > 0 );
        state_t current_state = (state_t)j;
        printf("Current: State[%d] = %s\n", j, state_str[j]); 

        state_t current_parent = current_state;
        while( current_parent != STATE_ROOT )
        {
            current_parent = parent_lookup[(int)current_parent];
            printf("\t %s\n", state_str[(int)current_parent]);
            current_nest++;
        }

        if( current_nest > nest_size )
        {
            nest_size = current_nest;
        }

        j--;
    }

    printf("Max State Nesting Size: %d\n", nest_size );
}

#define NESTED_STATES(x, ...) __VA_ARGS__,

#define _NUM_LEVELS( _0, _1, _2, _3, ... ) _3
#define NUM_LEVELS(...) _NUM_LEVELS( __VA_ARGS__ 3, 2, 1, 0 )

#define LEVEL_ENUM_(x) LEVEL_##x,
#define STATE_LEVEL( level, ... ) LEVEL_ENUM_(level)

#define NESTED(X) \
    X( 0U, root ) \
    X( 1U, a, b ) \
    X( 2U, a0, a1, b0 ) \

#define _INITIAL_STATES(...) __VA_ARGS__ 

typedef enum
{
    NESTED(STATE_LEVEL)
    count,
} enum_t;

typedef enum
{
    NESTED(NESTED_STATES)
    state_count,
}
states_t;

typedef struct state_machine_t state_machine_t;
typedef struct state_func_t ( *state_func_t ) ( state_machine_t * this, signal_t s );

struct state_machine_t 
{
    state_func_t parent;
    state_func_t state;
};



#define NEST_LEVELS NUM_LEVELS( NESTED( STATE_LEVEL) )

#define PARENT( parent ) state->parent = parent, ret = fsm_Unhandled
#define TRANSITION( new_state ) state->state = new_state, ret = fsm_Transition
#define HANDLED() ret = state->state = __func__, ret = fsm_Handled

#if   ( NEST_LEVELS > 1 )
#pragma message ("Hierarchical State Machine")
#elif (NEST_LEVELS == 1 )
#pragma message ("Finite (flat) State Machine")
#else
_Static_assert(false, "Bad state definitions")
#endif

static const uint32_t num_states = NEST_LEVELS;

static const states_t state_stack[NEST_LEVELS] = { _INITIAL_STATES( root, a, b) };

int main( void )
{
    printf("X-Macro FSM Example\n");

    state_t current_state = STATE_A0;
    
    Dispatch( &current_state, signal_Tick ); 
    Dispatch( &current_state, signal_Tick ); 
    Dispatch( &current_state, signal_TransitionToB ); 

    return 0;
}
