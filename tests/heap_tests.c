
#include "unity.h"
#include "heap_base.h"
#include <string.h>


static void CreateHeap(heap_t * heap, uint32_t * data, uint32_t len)
{
    assert(len <= heap->max);

    for(uint32_t idx = 0; idx < len; idx++)
    {
        Heap_Push(heap, data[idx]);
    }
}

static void test_Heap_Init(void)
{
    heap_t heap;
    Heap_Init(&heap);
    
    TEST_ASSERT_EQUAL( 0U, heap.fill );
    TEST_ASSERT_EQUAL( HEAP_LEN, heap.max );

}

static void test_Heap_Push(void)
{
    heap_t heap;
    Heap_Init(&heap);

    Heap_Push(&heap, 10U);
    
    TEST_ASSERT_EQUAL( 1U, heap.fill );
    TEST_ASSERT_EQUAL( HEAP_LEN, heap.max );
    TEST_ASSERT_EQUAL( 10U, heap.heap[0] );
}

static void test_Heap_Push2Gtr(void)
{
    heap_t heap;
    Heap_Init(&heap);

    Heap_Push(&heap, 10U);
    Heap_Push(&heap, 11U);
    
    TEST_ASSERT_EQUAL( 2U, heap.fill );
    TEST_ASSERT_EQUAL( HEAP_LEN, heap.max );
    TEST_ASSERT_EQUAL( 10U, heap.heap[0] );
    TEST_ASSERT_EQUAL( 11U, heap.heap[1] );
}

static void test_Heap_Push2Less(void)
{
    heap_t heap;
    Heap_Init(&heap);

    Heap_Push(&heap, 10U);
    Heap_Push(&heap, 9U);
    
    TEST_ASSERT_EQUAL( 2U, heap.fill );
    TEST_ASSERT_EQUAL( HEAP_LEN, heap.max );
    TEST_ASSERT_EQUAL( 9U, heap.heap[0] );
    TEST_ASSERT_EQUAL( 10U, heap.heap[1] );
}

static void test_Heap_Push3GL(void)
{
    heap_t heap;
    Heap_Init(&heap);

    Heap_Push(&heap, 10U);
    Heap_Push(&heap, 11U);
    Heap_Push(&heap, 9U);
    
    TEST_ASSERT_EQUAL( 3U, heap.fill );
    TEST_ASSERT_EQUAL( HEAP_LEN, heap.max );
    TEST_ASSERT_EQUAL( 9U, heap.heap[0] );
    TEST_ASSERT_EQUAL( 11U, heap.heap[1] );
    TEST_ASSERT_EQUAL( 10U, heap.heap[2] );
}

static void test_Heap_Push3GG(void)
{
    heap_t heap;
    Heap_Init(&heap);

    Heap_Push(&heap, 10U);
    Heap_Push(&heap, 11U);
    Heap_Push(&heap, 11U);
    
    TEST_ASSERT_EQUAL( 3U, heap.fill );
    TEST_ASSERT_EQUAL( HEAP_LEN, heap.max );
    TEST_ASSERT_EQUAL( 10U, heap.heap[0] );
    TEST_ASSERT_EQUAL( 11U, heap.heap[1] );
    TEST_ASSERT_EQUAL( 11U, heap.heap[2] );
}

static void test_Heap_Push3LG(void)
{
    heap_t heap;
    Heap_Init(&heap);

    Heap_Push(&heap, 10U);
    Heap_Push(&heap, 9U);
    Heap_Push(&heap, 11U);
    
    TEST_ASSERT_EQUAL( 3U, heap.fill );
    TEST_ASSERT_EQUAL( HEAP_LEN, heap.max );
    TEST_ASSERT_EQUAL( 9U, heap.heap[0] );
    TEST_ASSERT_EQUAL( 10U, heap.heap[1] );
    TEST_ASSERT_EQUAL( 11U, heap.heap[2] );
}

static void test_Heap_Push3LL(void)
{
    heap_t heap;
    Heap_Init(&heap);

    Heap_Push(&heap, 10U);
    Heap_Push(&heap, 9U);
    Heap_Push(&heap, 8U);
    
    TEST_ASSERT_EQUAL( 3U, heap.fill );
    TEST_ASSERT_EQUAL( HEAP_LEN, heap.max );
    TEST_ASSERT_EQUAL( 8U, heap.heap[0] );
    TEST_ASSERT_EQUAL( 10U, heap.heap[1] );
    TEST_ASSERT_EQUAL( 9U, heap.heap[2] );
}

static void test_Heap_Push4LLLL(void)
{
    heap_t heap;
    Heap_Init(&heap);

    Heap_Push(&heap, 10U);
    Heap_Push(&heap, 9U);
    Heap_Push(&heap, 8U);
    Heap_Push(&heap, 7U);
    
    TEST_ASSERT_EQUAL( 4U, heap.fill );
    TEST_ASSERT_EQUAL( HEAP_LEN, heap.max );
    TEST_ASSERT_EQUAL( 7U, heap.heap[0] );
    TEST_ASSERT_EQUAL( 8U, heap.heap[1] );
    TEST_ASSERT_EQUAL( 9U, heap.heap[2] );
    TEST_ASSERT_EQUAL( 10U, heap.heap[3] );
}

static void test_Heap_Push4LLLG(void)
{
    heap_t heap;
    Heap_Init(&heap);

    Heap_Push(&heap, 10U);
    Heap_Push(&heap, 9U);
    Heap_Push(&heap, 8U);
    Heap_Push(&heap, 9U);
    
    TEST_ASSERT_EQUAL( 4U, heap.fill );
    TEST_ASSERT_EQUAL( HEAP_LEN, heap.max );
    TEST_ASSERT_EQUAL( 8U, heap.heap[0] );
    TEST_ASSERT_EQUAL( 9U, heap.heap[1] );
    TEST_ASSERT_EQUAL( 9U, heap.heap[2] );
    TEST_ASSERT_EQUAL( 10U, heap.heap[3] );
}

static void test_Heap_Push4LLGL(void)
{
    heap_t heap;
    Heap_Init(&heap);

    Heap_Push(&heap, 10U);
    Heap_Push(&heap, 9U);
    Heap_Push(&heap, 11U);
    Heap_Push(&heap, 8U);
    
    TEST_ASSERT_EQUAL( 4U, heap.fill );
    TEST_ASSERT_EQUAL( HEAP_LEN, heap.max );
    TEST_ASSERT_EQUAL( 8U, heap.heap[0] );
    TEST_ASSERT_EQUAL( 9U, heap.heap[1] );
    TEST_ASSERT_EQUAL( 11U, heap.heap[2] );
    TEST_ASSERT_EQUAL( 10U, heap.heap[3] );
}

static void test_Heap_Push4LLGG(void)
{
    heap_t heap;
    Heap_Init(&heap);

    Heap_Push(&heap, 10U);
    Heap_Push(&heap, 9U);
    Heap_Push(&heap, 11U);
    Heap_Push(&heap, 12U);
    
    TEST_ASSERT_EQUAL( 4U, heap.fill );
    TEST_ASSERT_EQUAL( HEAP_LEN, heap.max );
    TEST_ASSERT_EQUAL( 9U, heap.heap[0] );
    TEST_ASSERT_EQUAL( 10U, heap.heap[1] );
    TEST_ASSERT_EQUAL( 11U, heap.heap[2] );
    TEST_ASSERT_EQUAL( 12U, heap.heap[3] );
}

static void test_Heap_Pop(void)
{
    heap_t heap;
    Heap_Init(&heap);

    uint32_t data = 10U;
    CreateHeap(&heap, &data, 1U);
    
    TEST_ASSERT_EQUAL( 1U, heap.fill );
    TEST_ASSERT_EQUAL( HEAP_LEN, heap.max );
    TEST_ASSERT_EQUAL( 10U, heap.heap[0] );

    uint32_t out = Heap_Pop(&heap);
    
    TEST_ASSERT_EQUAL( data, out );
    TEST_ASSERT_EQUAL( 0U, heap.fill );
    TEST_ASSERT_EQUAL( HEAP_LEN, heap.max );
}

static void test_Heap_Pop2G(void)
{
    heap_t heap;
    Heap_Init(&heap);

    uint32_t data[2] = {10U, 11U};
    CreateHeap(&heap, data, 2U);
    
    TEST_ASSERT_EQUAL( 2U, heap.fill );
    TEST_ASSERT_EQUAL( HEAP_LEN, heap.max );
    TEST_ASSERT_EQUAL( 10U, heap.heap[0] );
    TEST_ASSERT_EQUAL( 11U, heap.heap[1] );

    uint32_t out = Heap_Pop(&heap);
    
    TEST_ASSERT_EQUAL( data[0], out );
    TEST_ASSERT_EQUAL( data[1], heap.heap[0] );
    TEST_ASSERT_EQUAL( 1U, heap.fill );
    TEST_ASSERT_EQUAL( HEAP_LEN, heap.max );
}

static void test_Heap_Pop2L(void)
{
    heap_t heap;
    Heap_Init(&heap);

    uint32_t data[2] = {10U, 9U};
    CreateHeap(&heap, data, 2U);
    
    TEST_ASSERT_EQUAL( 2U, heap.fill );
    TEST_ASSERT_EQUAL( HEAP_LEN, heap.max );
    TEST_ASSERT_EQUAL( 9U, heap.heap[0] );
    TEST_ASSERT_EQUAL( 10U, heap.heap[1] );

    uint32_t out = Heap_Pop(&heap);
    
    TEST_ASSERT_EQUAL( data[1], out );
    TEST_ASSERT_EQUAL( data[0], Heap_Peek(&heap) );
    TEST_ASSERT_EQUAL( 1U, heap.fill );
    TEST_ASSERT_EQUAL( HEAP_LEN, heap.max );
}

static void test_Heap_Pop3LG(void)
{
    heap_t heap;
    Heap_Init(&heap);

    uint32_t data[3] = {10U, 9U, 11U};
    CreateHeap(&heap, data, 3U);
    
    TEST_ASSERT_EQUAL( 3U, heap.fill );
    TEST_ASSERT_EQUAL( HEAP_LEN, heap.max );
    TEST_ASSERT_EQUAL( 9U, heap.heap[0] );
    TEST_ASSERT_EQUAL( 10U, heap.heap[1] );
    TEST_ASSERT_EQUAL( 11U, heap.heap[2] );

    uint32_t out = Heap_Pop(&heap);
    
    TEST_ASSERT_EQUAL( data[1], out );
    TEST_ASSERT_EQUAL( data[0], Heap_Peek(&heap) );
    TEST_ASSERT_EQUAL( 2U, heap.fill );
    TEST_ASSERT_EQUAL( HEAP_LEN, heap.max );
}

static void test_Heap_Pop3GG(void)
{
    heap_t heap;
    Heap_Init(&heap);

    uint32_t data[3] = {10U, 11U, 12U};
    CreateHeap(&heap, data, 3U);
    
    TEST_ASSERT_EQUAL( 3U, heap.fill );
    TEST_ASSERT_EQUAL( HEAP_LEN, heap.max );
    TEST_ASSERT_EQUAL( 10U, heap.heap[0] );
    TEST_ASSERT_EQUAL( 11U, heap.heap[1] );
    TEST_ASSERT_EQUAL( 12U, heap.heap[2] );

    uint32_t out = Heap_Pop(&heap);
    
    TEST_ASSERT_EQUAL( 10U, out );
    TEST_ASSERT_EQUAL( 11U, Heap_Peek(&heap) );
    TEST_ASSERT_EQUAL( 12U, heap.heap[1] );
    TEST_ASSERT_EQUAL( 2U, heap.fill );
    TEST_ASSERT_EQUAL( HEAP_LEN, heap.max );
}

static void test_Heap_Pop3GL(void)
{
    heap_t heap;
    Heap_Init(&heap);

    uint32_t data[3] = {10U, 11U, 9U};
    CreateHeap(&heap, data, 3U);
    
    TEST_ASSERT_EQUAL( 3U, heap.fill );
    TEST_ASSERT_EQUAL( HEAP_LEN, heap.max );
    TEST_ASSERT_EQUAL( 9U, heap.heap[0] );
    TEST_ASSERT_EQUAL( 11U, heap.heap[1] );
    TEST_ASSERT_EQUAL( 10U, heap.heap[2] );

    uint32_t out = Heap_Pop(&heap);
    
    TEST_ASSERT_EQUAL( 9U, out );
    TEST_ASSERT_EQUAL( 10U, Heap_Peek(&heap) );
    TEST_ASSERT_EQUAL( 11U, heap.heap[1] );
    TEST_ASSERT_EQUAL( 2U, heap.fill );
    TEST_ASSERT_EQUAL( HEAP_LEN, heap.max );
}

static void test_Heap_Scenario0(void)
{
    heap_t heap;
    Heap_Init(&heap);

    uint32_t data[8] = {85,31,57,4,81,86,12,55};
    uint32_t sorted[8] = {4,12,31,55,57,81,85,86};
    CreateHeap(&heap, data, 8U);
    
    TEST_ASSERT_EQUAL( 8U, heap.fill );
    TEST_ASSERT_EQUAL( HEAP_LEN, heap.max );
    TEST_ASSERT_EQUAL(  4U, heap.heap[0] );
    TEST_ASSERT_EQUAL( 31U, heap.heap[1] );
    TEST_ASSERT_EQUAL( 12U, heap.heap[2] );
    TEST_ASSERT_EQUAL( 55U, heap.heap[3] );
    TEST_ASSERT_EQUAL( 81U, heap.heap[4] );
    TEST_ASSERT_EQUAL( 86U, heap.heap[5] );
    TEST_ASSERT_EQUAL( 57U, heap.heap[6] );
    TEST_ASSERT_EQUAL( 85U, heap.heap[7] );

    uint32_t out = Heap_Pop(&heap);
    
    TEST_ASSERT_EQUAL( 4U, out );
    TEST_ASSERT_EQUAL( 12U, heap.heap[0] );
    TEST_ASSERT_EQUAL( 31U, heap.heap[1] );
    TEST_ASSERT_EQUAL( 57U, heap.heap[2] );
    TEST_ASSERT_EQUAL( 55U, heap.heap[3] );
    TEST_ASSERT_EQUAL( 81U, heap.heap[4] );
    TEST_ASSERT_EQUAL( 86U, heap.heap[5] );
    TEST_ASSERT_EQUAL( 85U, heap.heap[6] );
    
    TEST_ASSERT_EQUAL( 7U, heap.fill );
    TEST_ASSERT_EQUAL( HEAP_LEN, heap.max );

    for( uint32_t idx = 1U; idx < 7U; idx++)
    {
        TEST_ASSERT_EQUAL(sorted[idx], Heap_Pop(&heap));
    }
}

extern void HeapTestSuite(void)
{
    RUN_TEST(test_Heap_Init);
    RUN_TEST(test_Heap_Push);
    RUN_TEST(test_Heap_Push2Gtr);
    RUN_TEST(test_Heap_Push2Less);
    RUN_TEST(test_Heap_Push3GL);
    RUN_TEST(test_Heap_Push3GG);
    RUN_TEST(test_Heap_Push3LG);
    RUN_TEST(test_Heap_Push3LL);
    RUN_TEST(test_Heap_Push4LLLL);
    RUN_TEST(test_Heap_Push4LLLG);
    RUN_TEST(test_Heap_Push4LLGL);
    RUN_TEST(test_Heap_Push4LLGG);
    RUN_TEST(test_Heap_Pop);
    RUN_TEST(test_Heap_Pop2G);
    RUN_TEST(test_Heap_Pop2L);
    RUN_TEST(test_Heap_Pop3LG);
    RUN_TEST(test_Heap_Pop3GG);
    RUN_TEST(test_Heap_Pop3GL);
    RUN_TEST(test_Heap_Scenario0);
}
