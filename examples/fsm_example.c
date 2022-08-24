#include <stdlib.h>
#include <stdio.h>
#include "../src/fsm.h"


/* Super States */
static fsm_status_t SuperState_A( fsm_t * this, signal s);
static fsm_status_t SuperState_B( fsm_t * this, signal s);

/* SubStates */
static fsm_status_t SubState_A0( fsm_t * this, signal s);
static fsm_status_t SubState_A1( fsm_t * this, signal s);
static fsm_status_t SubState_B0( fsm_t * this, signal s);

enum Signals
{
    signal_Tick = signal_Count,
    signal_TransitionToA,
    signal_TransitionToB,
    signal_TransitionToA0,
    signal_TransitionToA1,
    signal_TransitionToB0,
};


static fsm_status_t SuperState_A( fsm_t * this, signal s)
{
    STATE_PRINT( s );

    fsm_status_t status = FSM_SuperTransition( this, NULL );
    switch( s )
    {
        case signal_Enter:
        {
            printf("\t Enter Signal\n");
            status = fsm_Handled;
        }
            break;
        case signal_Exit:
        {
            printf("\t Exit Signal\n");
            status = fsm_Handled;
        }
            break;
        case signal_Tick:
            printf("\t Tick Signal\n");
            break;
        case signal_Traverse:
        {
        //    printf("\t Traverse Signal\n");
            break;
        }
        default:
        {
        }
            break;
    }

    return status;
}

static fsm_status_t SuperState_B( fsm_t * this, signal s)
{
    STATE_PRINT( s );
    
    fsm_status_t status = FSM_SuperTransition( this, NULL );
    switch( s )
    {
        case signal_Enter:
        {
            printf("\t Enter Signal\n");
            status = fsm_Handled;
        }
            break;
        case signal_Exit:
        {
            printf("\t Exit Signal\n");
            status = fsm_Handled;
        }
            break;
        case signal_Tick:
            printf("\t Tick Signal\n");
            status = fsm_Handled;
            break;
        case signal_TransitionToA0:
        {
            printf("\t TransitionToA0 Signal\n");
            status = FSM_Transition( this, SubState_A0 );
        }
            break;
        case signal_Traverse:
        default:
        {
        }
            break;
    }
    return status;
}

static fsm_status_t SubState_A0( fsm_t * this, signal s)
{
    STATE_PRINT( s );
    
    fsm_status_t status = FSM_SuperTransition( this, SuperState_A );
    switch( s )
    {
        case signal_Enter:
        {
            printf("\t Enter Signal\n");
            status = fsm_Handled;
        }
            break;
        case signal_Exit:
        {
            printf("\t Exit Signal\n");
            status = fsm_Handled;
        }
            break;
        case signal_TransitionToA1:
        {
            printf("\t TransitionToA1 Signal\n");
            status = FSM_Transition( this, SubState_A1 );
        }
            break;
        case signal_TransitionToB:
        {
            printf("\t TransitionToB Signal\n");
            status = FSM_Transition( this, SuperState_B );
        }
            break;
        case signal_TransitionToB0:
        {
            printf("\t TransitionToB0 Signal\n");
            status = FSM_Transition( this, SubState_B0 );
        }
            break;
        case signal_Traverse:
        default:
        {
        }
            break;
    }

    return status;
}
static fsm_status_t SubState_A1( fsm_t * this, signal s)
{
    STATE_PRINT( s );
    
    fsm_status_t status = FSM_SuperTransition( this, SuperState_A );
    switch( s )
    {
        case signal_Enter:
        {
            printf("\t Enter Signal\n");
            status = fsm_Handled;
        }
            break;
        case signal_Exit:
        {
            printf("\t Exit Signal\n");
            status = fsm_Handled;
        }
            break;
        case signal_TransitionToA0:
        {
            printf("\t TransitionToA0 Signal\n");
            status = FSM_Transition( this, SubState_A0 );
        }
            break;
        case signal_Traverse:
        default:
        {
        }
            break;
    }

    return status;
}
static fsm_status_t SubState_B0( fsm_t * this, signal s)
{
    STATE_PRINT( s );
    fsm_status_t status = FSM_SuperTransition( this, SuperState_B );
    switch( s )
    {
        case signal_Enter:
        {
            printf("\t Enter Signal\n");
            status = fsm_Handled;
        }
            break;
        case signal_Exit:
        {
            printf("\t Exit Signal\n");
            status = fsm_Handled;
        }
            break;
        case signal_TransitionToA0:
        {
            printf("\t TransitionToA0 Signal\n");
            status = FSM_Transition( this, SubState_A0 );
        }
            break;
        case signal_Traverse:
        default:
        {
        }
            break;
    }
    return status;
}

int main( void )
{
    fsm_t state_machine;

    state_machine.state = SubState_A0;

    FSM_Dispatch( &state_machine, signal_Tick );
    FSM_Dispatch( &state_machine, signal_TransitionToB );
    FSM_Dispatch( &state_machine, signal_TransitionToA0 );
    FSM_Dispatch( &state_machine, signal_TransitionToA1 );
    FSM_Dispatch( &state_machine, signal_Tick );
    FSM_Dispatch( &state_machine, signal_TransitionToA0 );
    FSM_Dispatch( &state_machine, signal_TransitionToB0 );
    FSM_Dispatch( &state_machine, signal_Tick );
    FSM_Dispatch( &state_machine, signal_TransitionToA0 );

    return 0;
}
