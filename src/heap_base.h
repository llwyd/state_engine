#include <assert.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define HEAP_LEN (8U)

typedef struct 
{
    uint32_t heap[HEAP_LEN];
    uint32_t fill;
    uint32_t max;
}
heap_t;

extern void Heap_Init(heap_t * heap);
extern void Heap_Push(heap_t * heap, uint32_t data);
extern uint32_t Heap_Pop(heap_t * heap);

