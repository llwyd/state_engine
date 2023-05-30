#ifndef FIFO_BASE_
#define FIFO_BASE_

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define FIFO_Enqueue(f, val) ((f)->data = (val), FIFO_EnQ((fifo_base_t *)(f)))
#define FIFO_Dequeue(f) ((FIFO_DeQ((fifo_base_t *)(f))), (f)->data)

#define ENQUEUE_BOILERPLATE(TYPE, BASE) \
    { \
        TYPE * fifo = ((TYPE *)(BASE)); \
        \
        fifo->queue[ fifo->base.write_index ] = fifo->data; \
        fifo->base.write_index++; \
        fifo->base.fill++; \
        fifo->base.write_index = ( fifo->base.write_index & ( fifo->base.max - 1U ) ); \
    }

#define DEQUEUE_BOILERPLATE(TYPE, BASE) \
    { \
        TYPE * fifo = ((TYPE *)(BASE)); \
        \
        fifo->data = fifo->queue[ fifo->base.read_index ]; \
        fifo->base.read_index++; \
        fifo->base.fill--; \
        fifo->base.read_index = ( fifo->base.read_index & ( fifo->base.max - 1U ) ); \
    }

#define FLUSH_BOILERPLATE(TYPE, BASE) \
    { \
        TYPE * fifo = ((TYPE *)(BASE)); \
        \
        fifo->base.read_index = 0U; \
        fifo->base.write_index = 0U; \
        fifo->base.fill = 0U; \
    }

typedef struct fifo_vfunc_t fifo_vfunc_t;
typedef struct 
{
    fifo_vfunc_t const * vfunc;
    uint32_t read_index;
    uint32_t write_index;
    uint32_t fill;
    uint32_t max;
}
fifo_base_t;

struct fifo_vfunc_t
{
    void (*enq)(fifo_base_t * const base);
    void (*deq)(fifo_base_t * const base);
    void (*flush)(fifo_base_t * const base);
};

inline static void FIFO_EnQ( fifo_base_t * const fifo )
{
    assert( fifo != NULL );
    assert( fifo->vfunc != NULL );
    assert( fifo->fill < fifo->max );
    
    (fifo->vfunc->enq)(fifo);
}

inline static void FIFO_DeQ( fifo_base_t * const fifo )
{
    assert( fifo != NULL );
    assert( fifo->vfunc != NULL );
    assert( fifo->fill > 0U );

    (fifo->vfunc->deq)(fifo);
}

inline static void FIFO_Flush( fifo_base_t * const fifo )
{
    assert( fifo != NULL );
    assert( fifo->vfunc != NULL );

    (fifo->vfunc->flush)(fifo);
}

extern void FIFO_Init( fifo_base_t * const fifo, uint32_t size );
extern bool FIFO_IsFull( fifo_base_t const * const fifo );
extern bool FIFO_IsEmpty( fifo_base_t const * const fifo );

#endif /* FIFO_BASE_ */

