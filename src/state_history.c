#include "state_history.h"
#include <string.h>
#define FIFO_LEN (64)

static void Enqueue( fifo_base_t * const base );
static void Dequeue( fifo_base_t * const base );
static void Flush( fifo_base_t * const base );

extern void History_Init( history_fifo_t * fifo )
{
    static const fifo_vfunc_t vfunc =
    {
        .enq = Enqueue,
        .deq = Dequeue,
        .flush = Flush,
    };
    FIFO_Init( (fifo_base_t *)fifo, FIFO_LEN );
    
    fifo->base.vfunc = &vfunc;
    memset(fifo->queue, 0x00, FIFO_LEN * sizeof(fifo->data));
}

static void Enqueue( fifo_base_t * const base )
{
    assert(base != NULL );
    ENQUEUE_BOILERPLATE( history_fifo_t, base );
}

static void Dequeue( fifo_base_t * const base )
{
    assert(base != NULL );
    DEQUEUE_BOILERPLATE( history_fifo_t, base );
}

static void Flush( fifo_base_t * const base )
{
    assert(base != NULL );
    FLUSH_BOILERPLATE( history_fifo_t, base );
}
