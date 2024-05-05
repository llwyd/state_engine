
#include "unity.h"
#include "heap_base.h"
#include <string.h>


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
}
