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

//cppcheck-suppress misra-c2012-20.10
#define SIGNAL_ENUM(x) event_##x,

//cppcheck-suppress misra-c2012-20.10
#define SIGNAL_ENUM_(x) event_##x

//cppcheck-suppress misra-c2012-20.10
#define RETURN(x) return_##x
#define EVENT(x) SIGNAL_ENUM_(x)

//cppcheck-suppress misra-c2012-20.10
#define STATE(x) State_##x
#define STATE_PROTOTYPE_(x) static state_ret_t STATE(x) ( state_t * this, event_t s );

//cppcheck-suppress misra-c2012-20.10
#define SIGNAL_STR_(x) #x
#define SIGNAL_LOOKUP_(x) [SIGNAL_ENUM_(x)] = SIGNAL_STR_(x),

#define GENERATE_SIGNALS( SIG ) \
    enum Signal \
    { \
        SIGNAL_ENUM_( Tick ) = SIGNAL_ENUM( DefaultCount ) \
        SIG( SIGNAL_ENUM ) \
        SIGNAL_ENUM_( EventCount ) \
    }

#define GENERATESIGNAL_STR_INGS( SIG ) \
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
#define PARENT( parent_state ) this->state = STATE(parent_state);  ret = RETURN( Unhandled )
//cppcheck-suppress misra-c2012-2.5
#define TRANSITION( new_state ) this->state = STATE(new_state);  ret = RETURN( Transition )
//cppcheck-suppress misra-c2012-2.5
#define HANDLED() ret = RETURN ( Handled )
//cppcheck-suppress misra-c2012-2.5
#define NO_PARENT() this->state = NULL; ret = RETURN( Unhandled )


/* Platform Specific Stuff */
#ifdef TARGET_ARM


#elif TARGET_ESP32
#error "Esp32 target not yet supported"

#else

#endif

/* Signal to send events to a given state */
typedef uint32_t event_t;

/* Default events for state machine */
enum DefaultSignals
{
    DEFAULT_SIGNALS(SIGNAL_ENUM)
    SIGNAL_ENUM(DefaultCount)
};

typedef enum
{
    #define RETURN_CODE(x) RETURN(x),
        STATE_RETURN_CODES
    #undef RETURN_CODE
}
state_ret_t;

/* Circular buffer for FSM events. */
typedef struct state_fifo_t state_fifo_t;

#define MAX_NESTED_STATES ( 3U )
#define FIFO_BUFFER_SIZE ( 32U )

struct state_fifo_t
{
    uint32_t read_index;
    uint32_t write_index;
    uint32_t fill;
    event_t event[ FIFO_BUFFER_SIZE ];
};

/* Forward declaration so that function pointer with state can return itself */
typedef struct state_t state_t;

/* Function pointer that holds the state to execute */
typedef state_ret_t ( *state_func_t ) ( state_t * this, event_t s );

struct state_t
{
    state_func_t state;
};

#ifdef UNIT_TESTS

    #define UNIT_TEST_HISTORY_SIZE ( 64U )

    typedef struct
    {
        state_func_t state;
        event_t event;
    }
    state_history_data_t;

    typedef struct
    {
        state_history_data_t data[ UNIT_TEST_HISTORY_SIZE ];
        uint32_t read_index;
        uint32_t write_index;
        uint32_t fill;
    }
    state_history_t;

    extern state_history_t * STATE_GetHistory ( void );
    extern void STATE_UnitTestInit(void);
    extern void STATE_InitEventBuffer( state_fifo_t * const fsm_event );
    extern uint32_t STATE_TraverseToRoot( state_t * const source, state_func_t path[ MAX_NESTED_STATES ] );
#else

#endif
extern void STATEMACHINE_Init( state_t * state, state_fifo_t * fsm_event, state_ret_t (*initial_state) ( state_t * this, event_t s ) );

/* Event Dispatchers */
extern void STATEMACHINE_Dispatch( state_t * state, event_t s );
extern void STATEMACHINE_FlatDispatch( state_t * state, event_t s );

/* Event queuing */
extern void STATEMACHINE_FlushEvents( state_fifo_t * const fsm_event );
extern void STATEMACHINE_AddEvent( state_fifo_t * const fsm_event, event_t s);
extern event_t STATEMACHINE_GetLatestEvent( state_fifo_t * const fsm_event );
extern bool STATEMACHINE_EventsAvailable( const state_fifo_t * const fsm_event );


#endif /* STATE_H_ */

