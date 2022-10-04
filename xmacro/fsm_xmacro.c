#include <stdio.h>
#include <stdlib.h>

typedef struct
{
    void (*fn)(void);
} state_machine_t;

typedef enum
{
    fsm_Handled,
    fsm_Transition,
    fsm_SuperTransition,
} state_codes_t;

#define ROOT_STATE \
    X( ROOT ) \

#define PARENT_STATES \
    X( A ) \
    X( B ) \

#define SUB_STATES \
    X( A0 ) \
    X( A1 ) \
    X( B0 ) \

#define ENUM_(x) STATE_##x
#define FUNC_(x) State_##x

typedef enum
{
    #define X(a) ENUM_(a),
        ROOT_STATE
        PARENT_STATES
        SUB_STATES
    #undef X
    STATE_COUNT
}
state_t;


#define X(a) void FUNC_(a) (void);
    PARENT_STATES
    SUB_STATES
#undef X


void StateA( void )
{

}

void StateB( void )
{

}

int main( void )
{
    printf("X-Macro FSM Example\n");
    return 0;
}
