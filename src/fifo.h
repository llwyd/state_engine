#ifndef FIFO_H_
#define FIFO_H_

#include <stdint.h>
#include <stdbool.h>
#include <assert.h>

typedef struct 
{
    uint32_t r_index;
    uint32_t w_index;
    uint32_t fill;
    uint32_t max;
}
fifo_base_t;

/* TODO: Critical Sections, check fill, flush, power of 2 check, > 0 check */

#define FIFO_PASS_PTR(x) x * fifo
#define FIFO_PASS_DATA(x) x data
#define FIFO_FUNC_INIT(x) FIFO_Init##x
#define FIFO_FUNC_DEQ(x) FIFO_DEQ##x
#define FIFO_FUNC_ENQ(x) FIFO_ENQ##x
#define FIFO_FUNC_PEEK(x) FIFO_PEEK##x
#define FIFO_FUNC_FULL(x) FIFO_ISFULL##x
#define FIFO_FUNC_NE(x) FIFO_NE##x

#define CREATE_FIFO( NAME, FIFO_TYPE, DATA_TYPE, LEN ) \
    _Static_assert( LEN > 0U, "FIFO Length must be greater than zero"); \
    _Static_assert( (LEN & (LEN - 1U )) == 0U, "FIFO Length must be power of 2"); \
    typedef struct \
    { \
        fifo_base_t base; \
        DATA_TYPE queue [(LEN)]; \
    } \
    FIFO_TYPE; \
    \
    \
    void FIFO_FUNC_INIT(NAME) (FIFO_PASS_PTR(FIFO_TYPE)) \
    { \
        fifo->base.max = LEN; \
        fifo->base.fill = 0U; \
        fifo->base.r_index = 0U; \
        fifo->base.w_index = 0U; \
    } \
    \
    DATA_TYPE FIFO_FUNC_DEQ(NAME) (FIFO_PASS_PTR(FIFO_TYPE)) \
    { \
        assert( fifo != NULL ); \
        assert( fifo->base.fill > 0U ); \
        \
        DATA_TYPE ret = fifo->queue[ fifo->base.r_index ]; \
        fifo->base.r_index++; \
        fifo->base.fill--; \
        fifo->base.r_index = ( fifo->base.r_index & ( LEN - 1U ) ); \
        \
        return ret; \
    } \
    \
    void FIFO_FUNC_ENQ(NAME) (FIFO_PASS_PTR(FIFO_TYPE), FIFO_PASS_DATA(DATA_TYPE)) \
    { \
        assert( fifo != NULL ); \
        assert( fifo->base.fill < LEN ); \
        fifo->queue[ fifo->base.w_index ] = data; \
        fifo->base.w_index++; \
        fifo->base.fill++; \
        fifo->base.w_index = ( fifo->base.w_index & ( LEN - 1U ) ); \
    } \
    \
    bool FIFO_FUNC_NE(NAME) (FIFO_PASS_PTR(FIFO_TYPE)) \
    { \
        return (fifo->base.fill > 0U ); \
    } \
    \
    bool FIFO_FUNC_FULL(NAME) (FIFO_PASS_PTR(FIFO_TYPE)) \
    { \
        return !(fifo->base.fill < LEN ); \
    } 

#endif /* FIFO_H_ */
