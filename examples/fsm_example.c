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

enum Signals
{
    signal_Tick = signal_Count,
    signal_TransitionToA,
    signal_TransitionToB,
    signal_TransitionToA0,
    signal_TransitionToA1,
    signal_TransitionToB0,
};


static state_ret_t SuperState_A( fsm_t * this, signal s)
{
    STATE_PRINT( s );
    state_ret_t ret;

    switch( s )
    {
        case signal_Enter:
            printf("\t Enter Signal\n");
            HANDLED( this );
            break;
        case signal_Exit:
            printf("\t Exit Signal\n");
            HANDLED( this );
            break;
        case signal_Tick:
            printf("\t Tick Signal\n");
            HANDLED( this );
            break;
        case signal_Traverse:
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
    STATE_PRINT( s );
    state_ret_t ret;
    
    switch( s )
    {
        case signal_Enter:
            printf("\t Enter Signal\n");
            HANDLED( this );
            break;
        case signal_Exit:
            printf("\t Exit Signal\n");
            HANDLED( this );
            break;
        case signal_Tick:
            printf("\t Tick Signal\n");
            HANDLED( this );
            break;
        case signal_TransitionToA0:
            printf("\t TransitionToA0 Signal\n");
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
    STATE_PRINT( s );
    state_ret_t ret;

    switch( s )
    {
        case signal_Enter:
            printf("\t Enter Signal\n");
            HANDLED( this );
            break;
        case signal_Exit:
            printf("\t Exit Signal\n");
            HANDLED( this );
            break;
        case signal_TransitionToA1:
            printf("\t TransitionToA1 Signal\n");
            TRANSITION( this, SubState_A1 );
            break;
        case signal_TransitionToB:
            printf("\t TransitionToB Signal\n");
            TRANSITION( this, SuperState_B );
            break;
        case signal_TransitionToB0:
            printf("\t TransitionToB0 Signal\n");
            TRANSITION( this, SubState_B0 );
            break;
        case signal_TransitionToA0:
            printf("\t TransitionToA0 Signal\n");
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
    STATE_PRINT( s );
    state_ret_t ret;
    
    switch( s )
    {
        case signal_Enter:
            printf("\t Enter Signal\n");
            HANDLED( this );
            break;
        case signal_Exit:
            printf("\t Exit Signal\n");
            HANDLED( this );
            break;
        case signal_TransitionToA0:
            printf("\t TransitionToA0 Signal\n");
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
    STATE_PRINT( s );
    state_ret_t ret;

    switch( s )
    {
        case signal_Enter:
            printf("\t Enter Signal\n");
            HANDLED( this );
            break;
        case signal_Exit:
            printf("\t Exit Signal\n");
            HANDLED( this );
            break;
        case signal_TransitionToA0:
            printf("\t TransitionToA0 Signal\n");
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

    FSM_HierarchicalDispatch( &state_machine, signal_Tick );
    FSM_HierarchicalDispatch( &state_machine, signal_TransitionToB );
    FSM_HierarchicalDispatch( &state_machine, signal_TransitionToA0 );
    FSM_HierarchicalDispatch( &state_machine, signal_TransitionToA1 );
    FSM_HierarchicalDispatch( &state_machine, signal_Tick );
    FSM_HierarchicalDispatch( &state_machine, signal_TransitionToA0 );
    FSM_HierarchicalDispatch( &state_machine, signal_TransitionToB0 );
    FSM_HierarchicalDispatch( &state_machine, signal_Tick );
    FSM_HierarchicalDispatch( &state_machine, signal_TransitionToA0 );
    FSM_HierarchicalDispatch( &state_machine, signal_TransitionToA0 );

    return 0;
}
