#include <stdlib.h>
#include <stdio.h>
#include "../src/fsm.h"


#define SIGNALS(SIG) \
    SIG( TransitionToA ) \
    SIG( TransitionToB ) \
    SIG( TransitionToA0 ) \
    SIG( TransitionToA1 ) \
    SIG( TransitionToB0 ) \

#define STATES(ST) \
    ST( A ) \
    ST( B ) \
    ST( A0 ) \
    ST( A1 ) \
    ST( B0 ) \
    

GENERATE_SIGNALS( SIGNALS );
GENERATE_SIGNAL_STRINGS( SIGNALS );
GENERATE_STATE_PROTOTYPES( STATES );

static state_ret_t State_A( state_t * this, signal s)
{
    STATE_DEBUG( s );
    state_ret_t ret;

    switch( s )
    {
        case EVENT(Enter):
            HANDLED( this );
            break;
        case EVENT(Exit):
            HANDLED( this );
            break;
        case EVENT(Tick):
            HANDLED( this );
            break;
        default:
        {
            NO_PARENT( this );
            break;
        }
    }

    return ret;
}

static state_ret_t State_B( state_t * this, signal s)
{
    STATE_DEBUG( s );
    state_ret_t ret;
    
    switch( s )
    {
        case EVENT(Enter):
            HANDLED( this );
            break;
        case EVENT(Exit):
            HANDLED( this );
            break;
        case EVENT(Tick):
            HANDLED( this );
            break;
        case EVENT(TransitionToA0):
            TRANSITION( this, A0 );
            break;
        default:
            NO_PARENT(this);
            break;
    }
    return ret;
}

static state_ret_t State_A0( state_t * this, signal s)
{
    STATE_DEBUG( s );
    state_ret_t ret;

    switch( s )
    {
        case EVENT(Enter):
            HANDLED( this );
            break;
        case EVENT(Exit):
            HANDLED( this );
            break;
        case EVENT(TransitionToA1):
            TRANSITION( this, A1 );
            break;
        case EVENT(TransitionToB):
            TRANSITION( this, B );
            break;
        case EVENT(TransitionToB0):
            TRANSITION( this, B0 );
            break;
        case EVENT(TransitionToA0):
            TRANSITION( this, A0 );
            break;
        default:
            PARENT( this, A );
            break;
    }

    return ret;
}

static state_ret_t State_A1( state_t * this, signal s )
{
    STATE_DEBUG( s );
    state_ret_t ret;
    
    switch( s )
    {
        case EVENT(Enter):
            HANDLED( this );
            break;
        case EVENT(Exit):
            HANDLED( this );
            break;
        case EVENT(TransitionToA0):
            TRANSITION( this, A0 );
            break;
        default:
            PARENT( this, A );
            break;
    }

    return ret;
}

static state_ret_t State_B0( state_t * this, signal s)
{
    STATE_DEBUG( s );
    state_ret_t ret;

    switch( s )
    {
        case EVENT(Enter):
            HANDLED( this );
            break;
        case EVENT(Exit):
            HANDLED( this );
            break;
        case EVENT(TransitionToA0):
            TRANSITION( this, A0 );
            break;
        default:
            PARENT( this, B );
            break;
    }
    return ret;
}

int main( void )
{
    state_t state_machine;

    state_machine.state = STATE( A0 );

    FSM_HierarchicalDispatch( &state_machine, EVENT(Tick ));
    FSM_HierarchicalDispatch( &state_machine, EVENT(TransitionToB ));
    FSM_HierarchicalDispatch( &state_machine, EVENT(TransitionToA0 ));
    FSM_HierarchicalDispatch( &state_machine, EVENT(TransitionToA1 ));
    FSM_HierarchicalDispatch( &state_machine, EVENT(Tick ));
    FSM_HierarchicalDispatch( &state_machine, EVENT(TransitionToA0 ));
    FSM_HierarchicalDispatch( &state_machine, EVENT(TransitionToB0 ));
    FSM_HierarchicalDispatch( &state_machine, EVENT(Tick ));
    FSM_HierarchicalDispatch( &state_machine, EVENT(TransitionToA0 ));
    FSM_HierarchicalDispatch( &state_machine, EVENT(TransitionToA0 ));

    return 0;
}
