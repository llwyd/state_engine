#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

typedef struct
{
    void (*fn)(void);
} state_machine_t;

typedef uint32_t signal_t;

typedef enum
{
    fsm_Handled,
    fsm_Unhandled, /* If unhandled, need to traverse up the states */
    fsm_Transition,
} state_return_t;

#define STATES \
    X( A, ROOT ) \
    X( B, ROOT ) \
    X( A0, A ) \
    X( A1, A ) \
    X( B0, A ) \

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

#define FUNC_RETURN  state_return_t 
#define FUNC_ARGS ( signal_t s )

typedef enum
{
    STATE_ROOT,
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

state_return_t StateB( signal_t s )
{
    printf("Func: %s\n", __func__);
    state_return_t ret = fsm_Handled;
    return ret;
}

int main( void )
{
    printf("X-Macro FSM Example\n");
 //   signal_t s;
    func_lookup[STATE_A]( signal_Entry );
    return 0;
}
