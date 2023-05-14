#include "fifo_tests.h"
#include "fifo.h"
#include "unity.h"

CREATE_FIFO(UInt32,test_fifo_t, uint32_t, 32 );

void test_FIFO_Init(void)
{
    test_fifo_t fifo;
    FIFO_InitUInt32(&fifo);

    TEST_ASSERT_EQUAL( 32U, fifo.base.max );
    TEST_ASSERT_EQUAL( 0U, fifo.base.fill );
    TEST_ASSERT_EQUAL( 0U, fifo.base.r_index );
    TEST_ASSERT_EQUAL( 0U, fifo.base.w_index );
}

void test_FIFO_Enqueue(void)
{
    test_fifo_t fifo;
    FIFO_InitUInt32(&fifo);
    FIFO_ENQUInt32(&fifo, 0x12345678);

    TEST_ASSERT_EQUAL( 32U, fifo.base.max );
    TEST_ASSERT_EQUAL( 1U, fifo.base.fill );
    TEST_ASSERT_EQUAL( 0U, fifo.base.r_index );
    TEST_ASSERT_EQUAL( 1U, fifo.base.w_index );
    TEST_ASSERT_EQUAL( 0x12345678, fifo.queue[0U]);
}

void test_FIFO_Dequeue(void)
{
    test_fifo_t fifo;
    FIFO_InitUInt32(&fifo);
    FIFO_ENQUInt32(&fifo, 0x12345678);
    TEST_ASSERT_EQUAL( 32U, fifo.base.max );
    TEST_ASSERT_EQUAL( 1U, fifo.base.fill );
    TEST_ASSERT_EQUAL( 0U, fifo.base.r_index );
    TEST_ASSERT_EQUAL( 1U, fifo.base.w_index );
    TEST_ASSERT_EQUAL( 0x12345678, fifo.queue[0U]);
    
    uint32_t value = FIFO_DEQUInt32(&fifo);

    TEST_ASSERT_EQUAL( 32U, fifo.base.max );
    TEST_ASSERT_EQUAL( 0U, fifo.base.fill );
    TEST_ASSERT_EQUAL( 1U, fifo.base.r_index );
    TEST_ASSERT_EQUAL( 1U, fifo.base.w_index );
    TEST_ASSERT_EQUAL( 0x12345678, value );
}

void test_FIFO_EnqueueMany(void)
{
    test_fifo_t fifo;
    FIFO_InitUInt32(&fifo);

    for( uint32_t idx = 0; idx < 32; idx++ )
    {
        FIFO_ENQUInt32(&fifo, 0x12345678);
        TEST_ASSERT_EQUAL( 32U, fifo.base.max );
        TEST_ASSERT_EQUAL( idx + 1U, fifo.base.fill );
        TEST_ASSERT_EQUAL( 0U, fifo.base.r_index );
        TEST_ASSERT_EQUAL( (idx + 1U)%32, fifo.base.w_index );
        TEST_ASSERT_EQUAL( 0x12345678, fifo.queue[idx]);
    }
}

void test_FIFO_DequeueMany(void)
{
    test_fifo_t fifo;
    FIFO_InitUInt32(&fifo);

    for( uint32_t idx = 0; idx < 32; idx++ )
    {
        FIFO_ENQUInt32(&fifo, 0x12345678);
    }

    TEST_ASSERT_EQUAL( 32U, fifo.base.fill );

    uint32_t currentFill = fifo.base.fill;
    for( uint32_t idx = 0; idx < 32; idx++ )
    {
        uint32_t value = FIFO_DEQUInt32(&fifo);
        currentFill--;
        TEST_ASSERT_EQUAL( 32U, fifo.base.max );
        TEST_ASSERT_EQUAL( currentFill, fifo.base.fill );
        TEST_ASSERT_EQUAL( (idx + 1U)%32, fifo.base.r_index );
        TEST_ASSERT_EQUAL( 0U, fifo.base.w_index );
        TEST_ASSERT_EQUAL( 0x12345678, value);

    }
}

void test_FIFO_NotEmpty(void)
{
    test_fifo_t fifo;
    FIFO_InitUInt32(&fifo);

    TEST_ASSERT_FALSE( FIFO_NEUInt32(&fifo));
    FIFO_ENQUInt32(&fifo,0xFFFFFFFF);
    TEST_ASSERT_TRUE( FIFO_NEUInt32(&fifo));
}

void test_FIFO_IsFull(void)
{
    test_fifo_t fifo;
    FIFO_InitUInt32(&fifo);

    for( uint32_t idx = 0; idx < 32; idx++ )
    {
        TEST_ASSERT_FALSE(FIFO_ISFULLUInt32(&fifo));
        FIFO_ENQUInt32(&fifo, 0x12345678);
    }
    TEST_ASSERT_TRUE(FIFO_ISFULLUInt32(&fifo));
}

extern void FIFOTestSuite(void)
{
    RUN_TEST(test_FIFO_Init);
    RUN_TEST(test_FIFO_Enqueue);
    RUN_TEST(test_FIFO_Dequeue);
    RUN_TEST(test_FIFO_EnqueueMany);
    RUN_TEST(test_FIFO_DequeueMany);
    RUN_TEST(test_FIFO_NotEmpty);
    RUN_TEST(test_FIFO_IsFull);
}

