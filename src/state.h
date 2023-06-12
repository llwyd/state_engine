/*
 *
 * FSM Engine
 *
 * T.L. 2022
 *
 */

#ifndef STATE_H_
#define STATE_H_

#include <stdint.h>
#include <stdbool.h>


/* Core State Machine Defines and helper macros */
#define DEFAULT_SIGNALS(SIGNAL) \
    SIGNAL( None ) \
    SIGNAL( Enter ) \
    SIGNAL( Exit ) \

#define STATE_RETURN_CODES \
    RETURN_CODE( None ) \
    RETURN_CODE( Handled ) \
    RETURN_CODE( Unhandled ) \
    RETURN_CODE( Transition ) \

#ifndef MAX_NESTED_STATES
#define MAX_NESTED_STATES ( 3U )
#endif /* MAX_NESTED_STATES */

#define SIGNAL_ENUM(x) event_##x,
#define SIGNAL_ENUM_(x) event_##x

#define RETURN(x) return_##x
#define EVENT(x) SIGNAL_ENUM_(x)

#define STATE(x) State_##x
#define STATE_PROTOTYPE_(x) static state_ret_t STATE(x) ( state_t * this, event_t s );

#define SIGNAL_STR_(x) #x
#define SIGNAL_LOOKUP_(x) [SIGNAL_ENUM_(x)] = SIGNAL_STR_(x),

#define GENERATE_SIGNALS( SIG ) \
    enum Signal \
    { \
        DEFAULT_SIGNALS(SIGNAL_ENUM) \
        SIG( SIGNAL_ENUM ) \
        SIGNAL_ENUM_( EventCount ) \
    }

#define GENERATE_SIGNAL_STRINGS( SIG ) \
    static const char *event_str[] = \
    { \
        DEFAULT_SIGNALS( SIGNAL_LOOKUP_ ) \
        SIGNAL_LOOKUP_( Tick ) \
        SIG( SIGNAL_LOOKUP_ ) \
    } \

#define GENERATE_STATE_PROTOTYPES( ST ) \
    ST( STATE_PROTOTYPE_ ) \


#define STATE_DEBUG( x ) printf("%s -> %s Event\n", __func__, event_str[x] )

//cppcheck-suppress misra-c2012-2.5
#define PARENT( X, parent_state ) ((X)->state = STATE(parent_state), RETURN( Unhandled ) )
//cppcheck-suppress misra-c2012-2.5
#define TRANSITION( X, new_state ) ((X)->state = STATE(new_state), RETURN( Transition ))
//cppcheck-suppress misra-c2012-2.5
#define HANDLED(X) RETURN ( Handled )
//cppcheck-suppress misra-c2012-2.5
#define NO_PARENT(X) ((X)->state = NULL,RETURN( Unhandled ))

/* Signal to send events to a given state */
typedef uint32_t event_t;

typedef enum
{
    #define RETURN_CODE(x) RETURN(x),
        STATE_RETURN_CODES
    #undef RETURN_CODE
}
state_ret_t;

/* Forward declaration so that function pointer with state can return itself */
typedef struct state_t state_t;

/* Function pointer that holds the state to execute */
typedef state_ret_t ( *state_func_t ) ( state_t * this, event_t s );

struct state_t
{
    state_func_t state;
};

extern void STATEMACHINE_Init( state_t * state, state_ret_t (*initial_state) ( state_t * this, event_t s ) );

/* Event Dispatchers */
extern void STATEMACHINE_Dispatch( state_t * state, event_t s );
extern void STATEMACHINE_FlatDispatch( state_t * state, event_t s );

#ifdef UNIT_TESTS
#include "fifo_base.h"
#include "state_history.h"
extern fifo_base_t * STATE_GetHistory ( void );
#endif

#endif /* STATE_H_ */

