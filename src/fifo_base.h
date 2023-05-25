#ifndef FIFO_BASE_
#define FIFO_BASE_

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

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
    void (*enq)(fifo_base_t * const fifo);
    void (*deq)(fifo_base_t * const fifo);
    void (*flush)(fifo_base_t * const fifo);
};

inline static void FIFO_EnQ( fifo_base_t * const fifo )
{
    assert( fifo != NULL );
    assert( fifo->vfunc != NULL );
}

inline static void FIFO_DeQ( fifo_base_t * const fifo )
{
    assert( fifo != NULL );
    assert( fifo->vfunc != NULL );
}

inline static void FIFO_Flush( fifo_base_t * const fifo )
{
    assert( fifo != NULL );
    assert( fifo->vfunc != NULL );

}

extern void FIFO_Init( fifo_base_t * const fifo, uint32_t size );
extern bool FIFO_IsFull( fifo_base_t const * const fifo );
extern bool FIFO_IsEmpty( fifo_base_t const * const fifo );

#endif /* FIFO_BASE_ */

