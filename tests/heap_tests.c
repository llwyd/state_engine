
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

extern void HeapTestSuite(void)
{
    RUN_TEST(test_Heap_Init);
}
