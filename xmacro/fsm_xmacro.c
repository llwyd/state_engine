#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct
{
    void (*fn)(void);
} state_machine_t;

typedef uint32_t signal_t;

enum Signals
{
    signal_Entry,
    signal_Exit,
    signal_Tick,
    signal_TransitionToA,
    signal_TransitionToB,
    signal_TransitionToA0,
    signal_TransitionToA1,
    signal_TransitionToB0,
};

typedef enum
{
    fsm_Handled,
    fsm_Unhandled, /* If unhandled, need to traverse up the states */
    fsm_Transition,
} state_return_t;

#define ROOT_STATE \
    X( ROOT ) \

#define PARENT_STATES \
    X( A ) \
    X( B ) \

#define SUB_STATES \
    X( A0 ) \
    X( A1 ) \
    X( B0 ) \

#define STATES \
    X( A, ROOT ) \
    X( B, ROOT ) \
    X( A0, A ) \
    X( A1, A ) \
    X( B0, A ) \

#define ENUM_(x) STATE_##x
#define FUNC_(x) State##x

#define FUNC_RETURN  state_return_t 
#define FUNC_ARGS ( signal_t s )

typedef enum
{
    STATE_ROOT,
    #define X(a) ENUM_(a),
        PARENT_STATES
        SUB_STATES
    #undef X
    STATE_COUNT
}
state_t;


#define X(a,b) FUNC_RETURN FUNC_(a) FUNC_ARGS;
    STATES
#undef X

const state_t parent_lookup[ ] =
{
    #define X(a, b) [ ENUM_(a) ] = ENUM_(b),
        STATES
    #undef X
};

state_return_t (*func_lookup[ ]) FUNC_ARGS =
{
    [ STATE_ROOT ] = NULL,
    #define X(a, b) [ ENUM_(a) ] = FUNC_(a),
        STATES
    #undef X
};

state_return_t StateB0( signal_t s )
{
    printf("Func: %s\n", __func__);
    state_return_t ret = fsm_Handled;
    return ret;
}

state_return_t StateA1( signal_t s )
{
    printf("Func: %s\n", __func__);
    state_return_t ret = fsm_Handled;
    return ret;
}

state_return_t StateA0( signal_t s )
{
    printf("Func: %s\n", __func__);
    state_return_t ret = fsm_Handled;
    return ret;
}

state_return_t StateA( signal_t s )
{
    printf("Func: %s\n", __func__);
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

state_return_t StateB( signal_t s )
{
    printf("Func: %s\n", __func__);
    state_return_t ret = fsm_Handled;
    return ret;
}

int main( void )
{
    printf("X-Macro FSM Example\n");
    signal_t s;
    func_lookup[STATE_A]( signal_Entry );
    return 0;
}
