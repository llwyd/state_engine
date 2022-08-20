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
};

static fsm_status_t SuperState_A( fsm_t * this, signal s)
{
    printf("SuperState_A\n");
    return fsm_Handled;
}

static fsm_status_t SuperState_B( fsm_t * this, signal s)
{
    printf("SuperState_B\n");
    return fsm_Handled;
}

static fsm_status_t SubState_A0( fsm_t * this, signal s)
{
    printf("SubState_A0\n");
    return FSM_SuperTransition( this, SuperState_A );
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
    FSM_Dispatch( &state_machine, signal_Tick );
    FSM_Dispatch( &state_machine, signal_Tick );

    return 0;
}
