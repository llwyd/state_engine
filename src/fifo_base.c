#include "fifo_base.h"

static void virtual_EnQ( fifo_base_t * fifo );
static void virtual_DeQ( fifo_base_t * fifo );
static void virtual_Flush( fifo_base_t * fifo );

extern void FIFO_Init( fifo_base_t * fifo, uint32_t size )
{
    assert(size > 0U);
    assert((size & (size - 1U )) == 0U);
    
    static const fifo_vfunc_t vfunc =
    {
        .enq = virtual_EnQ,
        .deq = virtual_DeQ,
        .flush = virtual_Flush,
    };

    fifo->vfunc = &vfunc;
    fifo->fill = 0U;
    fifo->read_index = 0U;
    fifo->write_index = 0U;
    fifo->max = size;
}

static void virtual_EnQ( fifo_base_t * const fifo )
{
    (void)fifo;
    assert(false);
}

static void virtual_DeQ( fifo_base_t * const fifo )
{
    (void)fifo;
    assert(false);
}

static void virtual_Flush( fifo_base_t * const fifo )
{
    (void)fifo;
    assert(false);
}

extern bool FIFO_IsFull( fifo_base_t const * const fifo )
{
    return ( fifo->fill == fifo->max );
}

extern bool FIFO_IsEmpty( fifo_base_t const * const fifo )
{
    return ( fifo->fill == 0U );
}

