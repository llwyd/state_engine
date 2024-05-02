#include "heap_base.h"
#include <string.h>

extern void Heap_Init(heap_t * heap)
{
    heap->fill = 0;
    heap->max = HEAP_LEN;
    memset(heap->heap, 0x00, sizeof(uint32_t) * HEAP_LEN);
}
extern void Heap_Push(heap_t * heap, uint32_t data);
extern uint32_t Heap_Pop(heap_t * heap);
