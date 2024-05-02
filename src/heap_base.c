#include "heap_base.h"
#include <string.h>
#include <stdio.h>

static void swap( uint32_t * a, uint32_t * b )
{
    assert(a != NULL);
    assert(b != NULL);

    uint32_t temp = *a;
    *a = *b;
    *b = temp;
}

extern void Heap_Init(heap_t * heap)
{
    heap->fill = 0;
    heap->max = HEAP_LEN;
    memset(heap->heap, 0x00, sizeof(uint32_t) * HEAP_LEN);
}

extern bool Heap_IsFull(heap_t * heap)
{
    return (heap->fill < heap->max);
}

extern void Heap_Push(heap_t * heap, uint32_t data)
{
    (void)heap;
    (void)data;
    
    assert(heap != NULL);
    assert(heap->fill < heap->max);

    /* Add the value to the bottom of the heap */
    heap->heap[heap->fill] = data;

    uint32_t idx = heap->fill;

    /* Swim through the heap to ensure heap order is correct */
    while( idx > 0U )
    {
        uint32_t parent = (idx - 1) >> 1U;
        if( heap->heap[idx] < heap->heap[parent] )
        {
            swap(&heap->heap[idx], &heap->heap[parent]);
        }
        idx = parent;
    }
    heap->fill++; 
}

extern uint32_t Heap_Pop(heap_t * heap);
