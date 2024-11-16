#include "emitter_tests.h"
#include "emitter_base.h"
#include "state.h"
#include "fifo_base.h"
#include "unity.h"
#include <string.h>

#define MAGIC_NUMBER_0 (0x12345678)
#define MAGIC_NUMBER_1 (0xA5A5A5A5)
#define MAGIC_NUMBER_2 (0x5A5A5A5A)

#define EVENTS(EVNT) \
    EVNT(TestEvent0) \
    EVNT(TestEvent1) \
    EVNT(TestEvent2) \

#define FIFO_LEN (8U)

GENERATE_EVENTS( EVENTS );

typedef struct
{
    fifo_base_t base;
    event_t queue[FIFO_LEN];
    event_t in;
    event_t out;
}
emitter_fifo_t;

typedef struct
{
    emitter_base_t base;
    uint32_t magic_0;
    uint32_t magic_1;
    uint32_t magic_2;
}
emitter_t;

static void Enqueue( fifo_base_t * const fifo );
static void Dequeue( fifo_base_t * const fifo );
static void Flush( fifo_base_t * const fifo );

static void Destroy(emitter_base_t * const base);
static void Create(emitter_base_t * const base, event_t event, uint32_t period);
static bool Emit(emitter_base_t * const base, event_t event);

static void Init( emitter_t * emitter, emitter_fifo_t * fifo )
{
    static const fifo_vfunc_t fifo_vfunc =
    {
        .enq = Enqueue,
        .deq = Dequeue,
        .flush = Flush,
    };
    FIFO_Init( (fifo_base_t *)fifo, FIFO_LEN );
    
    fifo->base.vfunc = &fifo_vfunc;
    fifo->in = 0x0;
    fifo->out = 0x0;
    memset(fifo->queue, 0x00, FIFO_LEN * sizeof(fifo->in));

    static const emitter_vfunc_t emitter_vfunc =
    {
        .emit = Emit,
        .create = Create,
        .destroy = Destroy,
    };
    Emitter_Init((emitter_base_t *)emitter, (fifo_base_t *)fifo);
    emitter->base.vfunc = &emitter_vfunc;
    emitter->magic_0 = MAGIC_NUMBER_0;
    emitter->magic_1 = MAGIC_NUMBER_1;
    emitter->magic_2 = MAGIC_NUMBER_2;
}

void Enqueue( fifo_base_t * const base )
{
    assert(base != NULL );
    ENQUEUE_BOILERPLATE( emitter_fifo_t, base );
}

void Dequeue( fifo_base_t * const base )
{
    assert(base != NULL );
    DEQUEUE_BOILERPLATE( emitter_fifo_t, base );
}

void Flush( fifo_base_t * const base )
{
    assert(base != NULL );
    FLUSH_BOILERPLATE( emitter_fifo_t, base );
}

static void Destroy(emitter_base_t * const base)
{
    assert(base!=NULL);
}

static void Create(emitter_base_t * const base, event_t event, uint32_t period)
{
    assert(base!=NULL);
    (void)event;
    (void)period;
}

static bool Emit(emitter_base_t * const base, event_t event)
{
    assert(base!=NULL);
    bool success = false;
    if(!FIFO_IsFull(base->fifo))
    {
        emitter_fifo_t * fifo = (emitter_fifo_t *)base->fifo;
        FIFO_Enqueue(fifo, event);
        success = true;
    }
    return success;
}

void test_EMITTER_Init(void)
{
    emitter_fifo_t fifo;
    emitter_t emitter;

    Init(&emitter, &fifo);

    TEST_ASSERT_EQUAL( FIFO_LEN, fifo.base.max );
    TEST_ASSERT_EQUAL( 0U, fifo.base.fill );
    TEST_ASSERT_EQUAL( 0U, fifo.base.read_index );
    TEST_ASSERT_EQUAL( 0U, fifo.base.write_index );
    TEST_ASSERT_EQUAL( 0U, fifo.in );
    TEST_ASSERT_EQUAL( 0U, fifo.out );

    emitter_fifo_t * fifo_ptr = (emitter_fifo_t *)emitter.base.fifo;

    TEST_ASSERT_EQUAL( FIFO_LEN, fifo_ptr->base.max );
    TEST_ASSERT_EQUAL( 0U, fifo_ptr->base.fill );
    TEST_ASSERT_EQUAL( 0U, fifo_ptr->base.read_index );
    TEST_ASSERT_EQUAL( 0U, fifo_ptr->base.write_index );
    TEST_ASSERT_EQUAL( 0U, fifo_ptr->in );
    TEST_ASSERT_EQUAL( 0U, fifo_ptr->out );

    TEST_ASSERT_EQUAL(MAGIC_NUMBER_0, emitter.magic_0);
    TEST_ASSERT_EQUAL(MAGIC_NUMBER_1, emitter.magic_1);
    TEST_ASSERT_EQUAL(MAGIC_NUMBER_2, emitter.magic_2);

    TEST_ASSERT_EQUAL(Emit, emitter.base.vfunc->emit);
    TEST_ASSERT_EQUAL(Create, emitter.base.vfunc->create);
    TEST_ASSERT_EQUAL(Destroy, emitter.base.vfunc->destroy);
    
    TEST_ASSERT_EQUAL(Flush, emitter.base.fifo->vfunc->flush);
    TEST_ASSERT_EQUAL(Enqueue, emitter.base.fifo->vfunc->enq);
    TEST_ASSERT_EQUAL(Dequeue, emitter.base.fifo->vfunc->deq);
}

void test_EMITTER_Emit(void)
{
    emitter_fifo_t fifo;
    emitter_t emitter;

    Init(&emitter, &fifo);

    Emitter_Emit(&emitter.base, EVENT(TestEvent0));
    
    emitter_fifo_t * fifo_ptr = (emitter_fifo_t *)emitter.base.fifo;

    TEST_ASSERT_EQUAL( FIFO_LEN, fifo_ptr->base.max );
    TEST_ASSERT_EQUAL( 1U, fifo_ptr->base.fill );
    TEST_ASSERT_EQUAL( 0U, fifo_ptr->base.read_index );
    TEST_ASSERT_EQUAL( 1U, fifo_ptr->base.write_index );
    TEST_ASSERT_EQUAL( EVENT(TestEvent0), fifo_ptr->in );
    TEST_ASSERT_EQUAL( 0U, fifo_ptr->out );
    
    TEST_ASSERT_EQUAL(MAGIC_NUMBER_0, emitter.magic_0);
    TEST_ASSERT_EQUAL(MAGIC_NUMBER_1, emitter.magic_1);
    TEST_ASSERT_EQUAL(MAGIC_NUMBER_2, emitter.magic_2);

    event_t event = FIFO_Dequeue( &fifo );
    TEST_ASSERT_EQUAL( event, EVENT( TestEvent0 ) );
    TEST_ASSERT_EQUAL( fifo.base.read_index, 1U );
    TEST_ASSERT_EQUAL( fifo.base.write_index, 1U );
    TEST_ASSERT_EQUAL( fifo.base.fill, 0U );
}

extern void EMITTERTestSuite(void)
{
    RUN_TEST(test_EMITTER_Init);
    RUN_TEST(test_EMITTER_Emit);
}
