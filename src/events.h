#ifndef EVENTS_H_
#define EVENTS_H_

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <state.h>
#include <string.h>
#include <fifo_base.h>

#ifndef FIFO_LEN
#define FIFO_LEN (32U)
#endif

typedef struct
{
    fifo_base_t base;
    state_event_t queue[FIFO_LEN];
    state_event_t in;
    state_event_t out;
} event_fifo_t;

extern void Events_Init(event_fifo_t * fifo);

extern void Events_Enqueue(event_fifo_t * fifo, state_t * s, event_t e);

#endif /* EVENTS_H_ */
