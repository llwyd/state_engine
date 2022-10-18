#include <stdlib.h>
#include <stdio.h>
#include "../src/fsm.h"

/* Super States */
static state_ret_t SuperState_A( fsm_t * this, signal s);
static state_ret_t SuperState_B( fsm_t * this, signal s);

/* SubStates */
static state_ret_t SubState_A0( fsm_t * this, signal s);
static state_ret_t SubState_A1( fsm_t * this, signal s);
static state_ret_t SubState_B0( fsm_t * this, signal s);

#define SIGNALS(SIG) \
    SIG( TransitionToA ) \
    SIG( TransitionToB ) \
    SIG( TransitionToA0 ) \
    SIG( TransitionToA1 ) \
    SIG( TransitionToB0 ) \

GENERATE_SIGNALS( SIGNALS );
GENERATE_SIGNAL_STRINGS( SIGNALS );

static state_ret_t SuperState_A( fsm_t * this, signal s)
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
            PARENT( this, NULL );
            break;
        }
    }

    return ret;
}

static state_ret_t SuperState_B( fsm_t * this, signal s)
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
            TRANSITION( this, SubState_A0 );
            break;
        default:
            PARENT(this, NULL );
            break;
    }
    return ret;
}

static state_ret_t SubState_A0( fsm_t * this, signal s)
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
            TRANSITION( this, SubState_A1 );
            break;
        case EVENT(TransitionToB):
            TRANSITION( this, SuperState_B );
            break;
        case EVENT(TransitionToB0):
            TRANSITION( this, SubState_B0 );
            break;
        case EVENT(TransitionToA0):
            TRANSITION( this, SubState_A0 );
            break;
        default:
            PARENT( this, SuperState_A );
            break;
    }

    return ret;
}
static state_ret_t SubState_A1( fsm_t * this, signal s)
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
            TRANSITION( this, SubState_A0 );
            break;
        default:
            PARENT( this, SuperState_A );
            break;
    }

    return ret;
}
static state_ret_t SubState_B0( fsm_t * this, signal s)
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
            TRANSITION( this, SubState_A0 );
            break;
        default:
            PARENT( this, SuperState_B );
            break;
    }
    return ret;
}

int main( void )
{
    fsm_t state_machine;

    state_machine.state = SubState_A0;

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
