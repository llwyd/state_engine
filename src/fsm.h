/*
 *
 * FSM Engine
 *
 * T.L. 2022
 *
 */

#ifndef _FSM_H_
#define _FSM_H_

#include <stdint.h>
#include <stdbool.h>

/* Global Settings */
#define STATE_ASSERT_ENABLE == false
#define STATE_DEBUG_ENABLE == false

/* Core State Machine Defines and helper macros */
#define DEFAULT_SIGNALS(SIGNAL) \
    SIGNAL( None ) \
    SIGNAL( Enter ) \
    SIGNAL( Exit ) \

#define STATE_RETURN_CODES \
    RETURN( None ) \
    RETURN( Handled ) \
    RETURN( Unhandled ) \
    RETURN( Transition ) \

#define SIGNAL_ENUM(x) event_##x,
#define _SIGNAL_ENUM(x) event_##x
#define RETURN_ENUM(x) return_##x
#define EVENT(x) _SIGNAL_ENUM(x)

#define STATE(x) State_##x
#define _STATE_PROTOTYPE(x) static state_ret_t STATE(x) ( state_t * this, event_t s );

#define _SIGNAL_STR(x) #x
#define _SIGNAL_LOOKUP(x) [_SIGNAL_ENUM(x)] = _SIGNAL_STR(x),

#define GENERATE_SIGNALS( SIG ) \
    enum Signal \
    { \
        _SIGNAL_ENUM( Tick ) = SIGNAL_ENUM( DefaultCount ) \
        SIG( SIGNAL_ENUM ) \
        _SIGNAL_ENUM( EventCount ) \
    }

#define GENERATE_SIGNAL_STRINGS( SIG ) \
    static const char *event_str[] = \
    { \
        DEFAULT_SIGNALS( _SIGNAL_LOOKUP ) \
        _SIGNAL_LOOKUP( Tick ) \
        SIG( _SIGNAL_LOOKUP ) \
    } \

#define GENERATE_STATE_PROTOTYPES( ST ) \
    ST( _STATE_PROTOTYPE ) \


#define STATE_DEBUG( x ) printf("%s -> %s Event\n", __func__, event_str[x] )


#define PARENT( parent_state ) this->state = STATE(parent_state);  ret = RETURN_ENUM( Unhandled )
#define TRANSITION( new_state ) this->state = STATE(new_state);  ret = RETURN_ENUM( Transition )
#define HANDLED() ret = RETURN_ENUM ( Handled )
#define NO_PARENT() this->state = NULL; ret = RETURN_ENUM( Unhandled )


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
    #define RETURN(x) RETURN_ENUM(x),
        STATE_RETURN_CODES
    #undef RETURN
}
state_ret_t;

/* Circular buffer for FSM events. */
typedef struct state_fifo_t state_fifo_t;

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
extern void STATE_InitEventBuffer( state_fifo_t * const fsm_event );

#else

#endif
extern void FSM_Init( state_t * state, state_fifo_t * fsm_event, state_ret_t (*initial_state) ( state_t * this, event_t s ) );

/* Event Dispatchers */
extern void FSM_HierarchicalDispatch( state_t * state, event_t s );
extern void FSM_Dispatch( state_t * state, event_t s );

/* Event queuing */
extern void FSM_FlushEvents( state_fifo_t * const fsm_event );
extern void FSM_AddEvent( state_fifo_t * const fsm_event, event_t s);
extern event_t FSM_GetLatestEvent( state_fifo_t * const fsm_event );
extern bool FSM_EventsAvailable( const state_fifo_t * const fsm_event );


#endif /* _FSM_H_ */

