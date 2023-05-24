#ifndef FIFO_BASE_
#define FIFO_BASE_

#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>



typedef struct 
{
    uint32_t r_index;
    uint32_t w_index;
    uint32_t fill;
    uint32_t max;
}
fifo_base_t;

inline static void FIFO_EnQ( fifo_base_t * fifo )
{
    assert( fifo != NULL );
}

inline static void FIFO_DeQ( fifo_base_t * fifo )
{
    assert( fifo != NULL );

}

extern void FIFO_Init( fifo_base_t * fifo );
extern void FIFO_Flush( fifo_base_t * fifo );
extern bool FIFO_HasSpace( fifo_base_t * fifo );
extern bool FIFO_IsEmpty( fifo_base_t * fifo );

#endif /* FIFO_BASE_ */

