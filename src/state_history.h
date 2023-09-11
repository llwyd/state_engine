#ifndef STATE_HISTORY_H
#define STATE_HISTORY_H

#include "state.h"
#include "fifo_base.h"
#define UNIT_TEST_HISTORY_SIZE ( 64U )

typedef struct
{
    state_func_t state;
    event_t event;
}
state_history_data_t;

typedef struct
{
    fifo_base_t base;
    state_history_data_t queue[UNIT_TEST_HISTORY_SIZE];
    state_history_data_t in;
    state_history_data_t out;
}
history_fifo_t;

extern void History_Init( history_fifo_t * fifo );
extern history_fifo_t * History_GetHistory ( void );

extern void STATE_UnitTestInit(void);
extern uint32_t STATE_TraverseToRoot( state_t * const source, state_func_t path[ MAX_NESTED_STATES ] );

#endif /* STATE_HISTORY_H */
