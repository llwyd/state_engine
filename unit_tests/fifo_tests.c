#include "fifo_tests.h"
#include "fifo.h"
#include "unity.h"

CREATE_FIFO(Int,test_fifo_t, uint32_t, 32 );

void test_FIFO_Init(void)
{
    test_fifo_t f;
    FIFO_InitInt(&f);

    TEST_ASSERT_EQUAL( 32U, f.base.max );
    TEST_ASSERT_EQUAL( 0U, f.base.fill );
    TEST_ASSERT_EQUAL( 0U, f.base.r_index );
    TEST_ASSERT_EQUAL( 0U, f.base.w_index );
}

extern int FIFOTestSuite(void)
{
    UNITY_BEGIN();
    RUN_TEST(test_FIFO_Init);

    return UNITY_END();

}

