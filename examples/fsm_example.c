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


static fsm_status_t SuperState_A( fsm_t * this, signal s)
{
    return fsm_Handled;
}

static fsm_status_t SuperState_B( fsm_t * this, signal s)
{
    return fsm_Handled;
}

static fsm_status_t SubState_A0( fsm_t * this, signal s)
{
    return fsm_Handled;
}
static fsm_status_t SubState_A1( fsm_t * this, signal s)
{
    return fsm_Handled;
}
static fsm_status_t SubState_B0( fsm_t * this, signal s)
{
    return fsm_Handled;
}

void Node_Init( hsm_t * node, state_func f, hsm_t * super )
{
    node->state = f;
    node->super_state = super;
}

int main( void )
{
    hsm_t state_machine[5];

    Node_Init( &state_machine[0], SuperState_A, NULL );
    Node_Init( &state_machine[1], SuperState_B, NULL );

    Node_Init( &state_machine[2], SubState_A0, &state_machine[0] );
    Node_Init( &state_machine[3], SubState_A1, &state_machine[0] );
    Node_Init( &state_machine[4], SubState_B0, &state_machine[1] );
}
