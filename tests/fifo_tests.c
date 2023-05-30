#include "fifo_tests.h"
#include "fifo_base.h"
//#include "fifo.h"
#include "unity.h"
#include <string.h>

#define FIFO_LEN (32U)

typedef struct
{
    fifo_base_t base;
    uint32_t queue[FIFO_LEN];
    uint32_t data;
} test_fifo_t;

static void Enqueue( fifo_base_t * const fifo );
static void Dequeue( fifo_base_t * const fifo );
static void Flush( fifo_base_t * const fifo );

void Init( test_fifo_t * fifo )
{
    static const fifo_vfunc_t vfunc =
    {
        .enq = Enqueue,
        .deq = Dequeue,
        .flush = Flush,
    };
    FIFO_Init( (fifo_base_t *)fifo, FIFO_LEN );
    
    fifo->base.vfunc = &vfunc;
    fifo->data = 0x0;
    memset(fifo->queue, 0x00, FIFO_LEN * sizeof(fifo->data));
}

void Enqueue( fifo_base_t * const base )
{
    assert(base != NULL );
    ENQUEUE_BOILERPLATE( test_fifo_t, base );
}

void Dequeue( fifo_base_t * const base )
{
    assert(base != NULL );
    DEQUEUE_BOILERPLATE( test_fifo_t, base );
}

void Flush( fifo_base_t * const base )
{
    assert(base != NULL );
}

void test_FIFO_Init(void)
{
    test_fifo_t fifo;
    FIFO_Init( (fifo_base_t *)&fifo, FIFO_LEN );
 
    TEST_ASSERT_EQUAL( FIFO_LEN, fifo.base.max );
    TEST_ASSERT_EQUAL( 0U, fifo.base.fill );
    TEST_ASSERT_EQUAL( 0U, fifo.base.read_index );
    TEST_ASSERT_EQUAL( 0U, fifo.base.write_index );
}

void test_FIFO_Enqueue(void)
{
    test_fifo_t fifo;
    Init(&fifo);

    FIFO_Enqueue(fifo, 0x12345678);

    TEST_ASSERT_EQUAL( 32U, fifo.base.max );
    TEST_ASSERT_EQUAL( 1U, fifo.base.fill );
    TEST_ASSERT_EQUAL( 0U, fifo.base.read_index );
    TEST_ASSERT_EQUAL( 1U, fifo.base.write_index );
    TEST_ASSERT_EQUAL( 0x12345678, fifo.queue[0U]);
}

void test_FIFO_Dequeue(void)
{
    test_fifo_t fifo;
    Init(&fifo);

    FIFO_Enqueue(fifo, 0x12345678);
    
    TEST_ASSERT_EQUAL( 32U, fifo.base.max );
    TEST_ASSERT_EQUAL( 1U, fifo.base.fill );
    TEST_ASSERT_EQUAL( 0U, fifo.base.read_index );
    TEST_ASSERT_EQUAL( 1U, fifo.base.write_index );
    TEST_ASSERT_EQUAL( 0x12345678, fifo.queue[0U]);
    
    uint32_t value = FIFO_Dequeue(fifo);

    TEST_ASSERT_EQUAL( 32U, fifo.base.max );
    TEST_ASSERT_EQUAL( 0U, fifo.base.fill );
    TEST_ASSERT_EQUAL( 1U, fifo.base.read_index );
    TEST_ASSERT_EQUAL( 1U, fifo.base.write_index );
    TEST_ASSERT_EQUAL( 0x12345678, value );
}


void test_FIFO_EnqueueMany(void)
{
    test_fifo_t fifo;
    Init(&fifo);

    for( uint32_t idx = 0; idx < 32; idx++ )
    {
        FIFO_Enqueue(fifo, 0x12345678);
        TEST_ASSERT_EQUAL( 32U, fifo.base.max );
        TEST_ASSERT_EQUAL( idx + 1U, fifo.base.fill );
        TEST_ASSERT_EQUAL( 0U, fifo.base.read_index );
        TEST_ASSERT_EQUAL( (idx + 1U)%32, fifo.base.write_index );
        TEST_ASSERT_EQUAL( 0x12345678, fifo.queue[idx]);
    }
}

void test_FIFO_DequeueMany(void)
{
    test_fifo_t fifo;
    Init(&fifo);

    for( uint32_t idx = 0; idx < 32; idx++ )
    {
        FIFO_Enqueue(fifo, 0x12345678);
    }

    TEST_ASSERT_EQUAL( 32U, fifo.base.fill );

    uint32_t currentFill = fifo.base.fill;
    for( uint32_t idx = 0; idx < 32; idx++ )
    {
        uint32_t value = FIFO_Dequeue(fifo);
        currentFill--;
        TEST_ASSERT_EQUAL( 32U, fifo.base.max );
        TEST_ASSERT_EQUAL( currentFill, fifo.base.fill );
        TEST_ASSERT_EQUAL( (idx + 1U)%32, fifo.base.read_index );
        TEST_ASSERT_EQUAL( 0U, fifo.base.write_index );
        TEST_ASSERT_EQUAL( 0x12345678, value);

    }
}

void test_FIFO_IsEmpty(void)
{
    test_fifo_t fifo;
    Init(&fifo);

    TEST_ASSERT_TRUE( FIFO_IsEmpty(&fifo.base));
    FIFO_Enqueue(fifo,0xFFFFFFFF);
    TEST_ASSERT_FALSE( FIFO_IsEmpty(&fifo.base));
}

void test_FIFO_IsFull(void)
{
    test_fifo_t fifo;
    Init(&fifo);

    for( uint32_t idx = 0; idx < 32; idx++ )
    {
        TEST_ASSERT_FALSE(FIFO_IsFull(&fifo.base));
        FIFO_Enqueue(fifo, 0x12345678);
    }
    TEST_ASSERT_TRUE(FIFO_IsFull(&fifo.base));
}

extern void FIFOTestSuite(void)
{
    RUN_TEST(test_FIFO_Init);
    RUN_TEST(test_FIFO_Enqueue);
    RUN_TEST(test_FIFO_Dequeue);
    RUN_TEST(test_FIFO_EnqueueMany);
    RUN_TEST(test_FIFO_DequeueMany);
    RUN_TEST(test_FIFO_IsEmpty);
    RUN_TEST(test_FIFO_IsFull);
}

