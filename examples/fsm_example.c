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
    signal_TransitionToB1,
};


static fsm_status_t SuperState_A( fsm_t * this, signal s)
{
    printf("SuperState_A\n");

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
    printf("SuperState_B\n");
    
    fsm_status_t status;
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
        case signal_TransitionToA0:
        {
            printf("\t TransitionToA0 Signal\n");
            status = FSM_Transition( this, SubState_A0 );
        }
            break;
        case signal_Traverse:
        default:
        {
            status = FSM_SuperTransition( this, NULL );
        }
            break;
    }
    return status;
}

static fsm_status_t SubState_A0( fsm_t * this, signal s)
{
    printf("SubState_A0\n");
    
    fsm_status_t status = fsm_Handled;
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
        case signal_TransitionToB:
        {
            printf("\t TransitionToB Signal\n");
            status = FSM_Transition( this, SuperState_B );
        }
            break;
        case signal_Traverse:
        default:
        {
            status = FSM_SuperTransition( this, SuperState_A );
        }
            break;
    }

    return status;
}
static fsm_status_t SubState_A1( fsm_t * this, signal s)
{
    printf("SubState_A1\n");
    return fsm_Handled;
}
static fsm_status_t SubState_B0( fsm_t * this, signal s)
{
    printf("SubState_B0\n");
    return fsm_Handled;
}

void Node_Init( hsm_t * node, state_func f, hsm_t * super )
{
    node->state = f;
    node->super_state = super;
}


void Init();

int main( void )
{
    fsm_t state_machine;

    state_machine.state = SubState_A0;

    FSM_Dispatch( &state_machine, signal_Tick );
    FSM_Dispatch( &state_machine, signal_TransitionToB );
    FSM_Dispatch( &state_machine, signal_TransitionToA0 );

    return 0;
}
