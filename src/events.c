#include "events.h"

static void Enqueue( fifo_base_t * const fifo );
static void Dequeue( fifo_base_t * const fifo );
static void Flush( fifo_base_t * const fifo );

extern void Events_Init(event_fifo_t * fifo)
{
    static const fifo_vfunc_t vfunc =
    {
        .enq = Enqueue,
        .deq = Dequeue,
        .flush = Flush,
    };
    FIFO_Init( (fifo_base_t *)fifo, FIFO_LEN );
    
    fifo->base.vfunc = &vfunc;

    state_event_t blank_event ={ .state = NULL, .event = 0U }; 
    fifo->in = blank_event;
    fifo->out = blank_event;
    memset(fifo->queue, 0x00, FIFO_LEN * sizeof(fifo->in));
}

extern void Events_Enqueue(event_fifo_t * fifo, state_t * s, event_t e)
{
    assert(fifo != NULL);
    assert(s != NULL);

    state_event_t event = {.state = s, .event = e};
    FIFO_Enqueue(fifo, event);
}

static void Enqueue( fifo_base_t * const base )
{
    assert(base != NULL );
    ENQUEUE_BOILERPLATE( event_fifo_t, base );
}

static void Dequeue( fifo_base_t * const base )
{
    assert(base != NULL );
    DEQUEUE_BOILERPLATE( event_fifo_t, base );
}

static void Flush( fifo_base_t * const base )
{
    assert(base != NULL );
    FLUSH_BOILERPLATE( event_fifo_t, base );
}

