#include "state_tests.h"
#include "state.h"
#include "unity.h"
#include <string.h>

#define SIGNALS(SIG) \
    SIG(Tick) \
    SIG( TransitionToA ) \
    SIG( TransitionToB ) \
    SIG( TransitionToA0 ) \
    SIG( TransitionToA1 ) \
    SIG( TransitionToB0 ) \
    SIG( TransitionToB1 ) 

GENERATE_SIGNALS( SIGNALS );
GENERATE_SIGNAL_STRINGS( SIGNALS );

DEFINE_STATE(A);
DEFINE_STATE(B);
DEFINE_STATE(A0);
DEFINE_STATE(A1);
DEFINE_STATE(B0);
DEFINE_STATE(B1);
DEFINE_STATE(C);


#define FIFO_LEN (32U)

typedef struct
{
    fifo_base_t base;
    event_t queue[FIFO_LEN];
    event_t data;
} event_fifo_t;

static void Enqueue( fifo_base_t * const fifo );
static void Dequeue( fifo_base_t * const fifo );
static void Flush( fifo_base_t * const fifo );

static void Init( event_fifo_t * fifo )
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
    ENQUEUE_BOILERPLATE( event_fifo_t, base );
}

void Dequeue( fifo_base_t * const base )
{
    assert(base != NULL );
    DEQUEUE_BOILERPLATE( event_fifo_t, base );
}

void Flush( fifo_base_t * const base )
{
    assert(base != NULL );
    FLUSH_BOILERPLATE( event_fifo_t, base );
}

static state_ret_t State_A( state_t * this, event_t s)
{
  state_ret_t ret;

  switch( s )
  {
    case EVENT(Enter):
      ret = HANDLED(this);
      break;
    case EVENT(Exit):
      ret = HANDLED(this);
      break;
    case EVENT(Tick):
      ret = HANDLED(this);
      break;
    case EVENT(TransitionToB):
      ret = TRANSITION( this, B );
      break;
    default:
      ret = NO_PARENT(this);
      break;
  }

  return ret;
}

static state_ret_t State_B( state_t * this, event_t s)
{
  state_ret_t ret;
  
  switch( s )
  {
    case EVENT(Enter):
      ret = HANDLED(this);
      break;
    case EVENT(Exit):
      ret = HANDLED(this);
      break;
    case EVENT(Tick):
      ret = HANDLED(this);
      break;
    case EVENT(TransitionToA0):
      ret = TRANSITION( this, A0 );
      break;
    default:
      ret = NO_PARENT(this);
      break;
  }
  return ret;
}

static state_ret_t State_A0( state_t * this, event_t s)
{
  state_ret_t ret;

  switch( s )
  {
    case EVENT(Enter):
      ret = HANDLED(this);
      break;
    case EVENT(Exit):
      ret = HANDLED(this);
      break;
    case EVENT(TransitionToA):
      ret = TRANSITION( this, A );
      break;
    case EVENT(TransitionToA1):
      ret = TRANSITION( this, A1 );
      break;
    case EVENT(TransitionToB):
      ret = TRANSITION( this, B );
      break;
    case EVENT(TransitionToB0):
      ret = TRANSITION( this, B0 );
      break;
    case EVENT(TransitionToA0):
      ret = TRANSITION( this, A0 );
      break;
    case EVENT(TransitionToB1):
      ret = TRANSITION( this, B1 );
      break;
    default:
      ret = PARENT( this, A );
      break;
  }

  return ret;
}

static state_ret_t State_A1( state_t * this, event_t s )
{
  state_ret_t ret;
  
  switch( s )
  {
    case EVENT(Enter):
      ret = HANDLED(this);
      break;
    case EVENT(Exit):
      ret = HANDLED(this);
      break;
    case EVENT(TransitionToA0):
      ret = TRANSITION( this, A0 );
      break;
    default:
      ret = PARENT( this, A );
      break;
  }

  return ret;
}

static state_ret_t State_B0( state_t * this, event_t s)
{
  state_ret_t ret;

  switch( s )
  {
    case EVENT(Enter):
      ret = HANDLED(this);
      break;
    case EVENT(Exit):
      ret = HANDLED(this);
      break;
    case EVENT(TransitionToA0):
      ret = TRANSITION( this, A0 );
      break;
    default:
      ret = PARENT( this, B );
      break;
  }
  return ret;
}

static state_ret_t State_B1( state_t * this, event_t s)
{
  state_ret_t ret;

  switch( s )
  {
    case EVENT(Enter):
      ret = TRANSITION( this, A1 );
      break;
    case EVENT(Exit):
      ret = HANDLED(this);
      break;
    case EVENT(TransitionToA0):
      ret = TRANSITION( this, A0 );
      break;
    default:
      ret = PARENT( this, B );
      break;
  }
  return ret;
}

static state_ret_t State_C( state_t * this, event_t s)
{
  state_ret_t ret;

  switch( s )
  {
    case EVENT(Enter):
      ret = HANDLED(this);
      break;
    case EVENT(Exit):
      ret = TRANSITION( this, A0 );
      break;
    case EVENT(Tick):
      ret = HANDLED(this);
      break;
    case EVENT(TransitionToB):
      ret = TRANSITION( this, B );
      break;
    default:
      ret = NO_PARENT(this);
      break;
  }

  return ret;
}

void setUp( void )
{

}

void tearDown( void )
{

}

static void test_STATE_Preprocessor( void )
{
    (void)event_str;
    TEST_ASSERT_EQUAL( EVENT(None),     0U);
    TEST_ASSERT_EQUAL( EVENT(Enter),    1U);
    TEST_ASSERT_EQUAL( EVENT(Exit),     2U);
    TEST_ASSERT_EQUAL( EVENT(Tick),     3U);
}

static void test_STATE_FIFOInit( void )
{
    event_fifo_t events;
    Init( &events );

    TEST_ASSERT_EQUAL( events.base.read_index, 0U );
    TEST_ASSERT_EQUAL( events.base.write_index, 0U );
    TEST_ASSERT_EQUAL( events.base.fill, 0U );
}

static void test_STATE_FIFOAddRemoveEvent( void )
{
    event_fifo_t events;
    Init( &events );

    TEST_ASSERT_EQUAL( events.base.read_index, 0U );
    TEST_ASSERT_EQUAL( events.base.write_index, 0U );
    TEST_ASSERT_EQUAL( events.base.fill, 0U );
    TEST_ASSERT_TRUE( FIFO_IsEmpty( &events.base ) );

    FIFO_Enqueue( &events, EVENT( Tick ) );
    TEST_ASSERT_EQUAL( events.base.read_index, 0U );
    TEST_ASSERT_EQUAL( events.base.write_index, 1U );
    TEST_ASSERT_EQUAL( events.base.fill, 1U );

    TEST_ASSERT_FALSE( FIFO_IsEmpty( &events.base ) );

    event_t event = FIFO_Dequeue( &events );
    TEST_ASSERT_EQUAL( event, EVENT( Tick ) );
    TEST_ASSERT_EQUAL( events.base.read_index, 1U );
    TEST_ASSERT_EQUAL( events.base.write_index, 1U );
    TEST_ASSERT_EQUAL( events.base.fill, 0U );
}

static void test_STATE_FIFOWrapAround( void )
{
    event_fifo_t events;
    Init( &events );
    
    events.base.write_index = ( events.base.max - 1U );
    events.base.read_index = ( events.base.max - 1U );
    events.base.fill = 0;

    FIFO_Enqueue( &events, EVENT( Tick ) );
    TEST_ASSERT_EQUAL( events.base.read_index, (events.base.max - 1U ));
    TEST_ASSERT_EQUAL( events.base.write_index, 0U );
    TEST_ASSERT_EQUAL( events.base.fill, 1U );
}

static void test_STATE_FIFOFlush( void )
{
    event_fifo_t events;
    Init( &events );
    
    FIFO_Enqueue( &events, EVENT( Tick ) );
    FIFO_Enqueue( &events, EVENT( Tick ) );
    FIFO_Enqueue( &events, EVENT( Tick ) );
    FIFO_Enqueue( &events, EVENT( Tick ) );

    TEST_ASSERT_EQUAL( events.base.read_index, 0U );
    TEST_ASSERT_EQUAL( events.base.write_index, 4U );
    TEST_ASSERT_EQUAL( events.base.fill, 4U );
    TEST_ASSERT_FALSE( FIFO_IsEmpty( &events.base ) );

    FIFO_Flush( &events.base );
    TEST_ASSERT_EQUAL( events.base.read_index, 4U );
    TEST_ASSERT_EQUAL( events.base.write_index, 4U );
    TEST_ASSERT_EQUAL( events.base.fill, 0U );
    TEST_ASSERT_TRUE( FIFO_IsEmpty( &events.base ) );
}

static void test_STATE_Init( void )
{
    STATE_UnitTestInit();

    state_t state;
    event_fifo_t events;

    fifo_base_t * history_base = STATE_GetHistory();
    history_fifo_t * history = (history_fifo_t*)history_base;

    /* State machine should
     * StateA -> Enter
     * StateA0 ->Enter
     */

    Init( &events );
    STATEMACHINE_Init( &state, STATE( A0 ) );
    
    TEST_ASSERT_EQUAL( history->base.fill, 2U ); 
    TEST_ASSERT_EQUAL( history->queue[0].state, STATE( A ) );
    TEST_ASSERT_EQUAL( history->queue[1].state, STATE( A0 ) );

    TEST_ASSERT_EQUAL( history->queue[0].event, EVENT( Enter ) );
    TEST_ASSERT_EQUAL( history->queue[1].event, EVENT( Enter ) );
    
    TEST_ASSERT_EQUAL( history->queue[2].state, NULL );
    TEST_ASSERT_EQUAL( history->queue[2].event, NULL );
}

static void test_STATE_SingleEvent( void )
{
    STATE_UnitTestInit();
    state_t state;
    fifo_base_t * history_base = STATE_GetHistory();
    history_fifo_t * history = (history_fifo_t*)history_base;

    state.state = STATE( A );

    STATEMACHINE_Dispatch( &state, EVENT( Tick ) );
    TEST_ASSERT_EQUAL( history->base.fill, 1U ); 
    TEST_ASSERT_EQUAL( history->queue[0].state, STATE( A ) );
    TEST_ASSERT_EQUAL( history->queue[1].state, NULL );
    TEST_ASSERT_EQUAL( history->queue[0].event, EVENT( Tick ) );
    TEST_ASSERT_EQUAL( history->queue[1].event, NULL );

    TEST_ASSERT_EQUAL( state.state, STATE( A ) );
}

static void test_STATE_SingleUnhandledEvent( void )
{
    STATE_UnitTestInit();
    state_t state;
    fifo_base_t * history_base = STATE_GetHistory();
    history_fifo_t * history = (history_fifo_t*)history_base;

    state.state = STATE( A0 );

    STATEMACHINE_Dispatch( &state, EVENT( Tick ) );
    TEST_ASSERT_EQUAL( history->base.fill, 2U ); 
    TEST_ASSERT_EQUAL( history->queue[0].state, STATE( A0 ) );
    TEST_ASSERT_EQUAL( history->queue[1].state, STATE ( A ) );
    TEST_ASSERT_EQUAL( history->queue[2].state, NULL );
    
    TEST_ASSERT_EQUAL( history->queue[0].event, EVENT( Tick ) );
    TEST_ASSERT_EQUAL( history->queue[1].event, EVENT( Tick ) );
    TEST_ASSERT_EQUAL( history->queue[2].event, NULL );

    TEST_ASSERT_EQUAL( state.state, STATE( A0 ) );
}

static void test_STATE_TransitionSharedParent( void )
{
    STATE_UnitTestInit();
    state_t state;
    fifo_base_t * history_base = STATE_GetHistory();
    history_fifo_t * history = (history_fifo_t*)history_base;

    state.state = STATE( A0 );

    STATEMACHINE_Dispatch( &state, EVENT( TransitionToA1 ) );
    TEST_ASSERT_EQUAL( history->base.fill, 3U ); 
    TEST_ASSERT_EQUAL( history->queue[0].state, STATE( A0 ) );
    TEST_ASSERT_EQUAL( history->queue[1].state, STATE( A0 ) );
    TEST_ASSERT_EQUAL( history->queue[2].state, STATE( A1 ) );
    TEST_ASSERT_EQUAL( history->queue[3].state, NULL );
    
    TEST_ASSERT_EQUAL( history->queue[0].event, EVENT( TransitionToA1 ) );
    TEST_ASSERT_EQUAL( history->queue[1].event, EVENT( Exit ) );
    TEST_ASSERT_EQUAL( history->queue[2].event, EVENT( Enter ) );
    TEST_ASSERT_EQUAL( history->queue[3].event, NULL );

    TEST_ASSERT_EQUAL( state.state, STATE( A1 ) );
    
    STATE_UnitTestInit();
    
    state.state = STATE( A );

    STATEMACHINE_Dispatch( &state, EVENT( TransitionToB ) );
    TEST_ASSERT_EQUAL( history->base.fill, 3U ); 
    TEST_ASSERT_EQUAL( history->queue[0].state, STATE( A ) );
    TEST_ASSERT_EQUAL( history->queue[1].state, STATE( A ) );
    TEST_ASSERT_EQUAL( history->queue[2].state, STATE( B ) );
    TEST_ASSERT_EQUAL( history->queue[3].state, NULL );
    
    TEST_ASSERT_EQUAL( history->queue[0].event, EVENT( TransitionToB ) );
    TEST_ASSERT_EQUAL( history->queue[1].event, EVENT( Exit ) );
    TEST_ASSERT_EQUAL( history->queue[2].event, EVENT( Enter ) );
    TEST_ASSERT_EQUAL( history->queue[3].event, NULL );

    TEST_ASSERT_EQUAL( state.state, STATE( B ) );
}

static void test_STATE_TransitionNoSharedParent( void )
{
    STATE_UnitTestInit();
    state_t state;
    fifo_base_t * history_base = STATE_GetHistory();
    history_fifo_t * history = (history_fifo_t*)history_base;

    state.state = STATE( A0 );

    STATEMACHINE_Dispatch( &state, EVENT( TransitionToB0 ) );
    TEST_ASSERT_EQUAL( history->base.fill, 5U ); 
    TEST_ASSERT_EQUAL( history->queue[0].state, STATE( A0 ) );
    TEST_ASSERT_EQUAL( history->queue[1].state, STATE( A0 ) );
    TEST_ASSERT_EQUAL( history->queue[2].state, STATE( A ) );
    TEST_ASSERT_EQUAL( history->queue[3].state, STATE( B ) );
    TEST_ASSERT_EQUAL( history->queue[4].state, STATE( B0 ) );
    TEST_ASSERT_EQUAL( history->queue[5].state, NULL );
    
    TEST_ASSERT_EQUAL( history->queue[0].event, EVENT( TransitionToB0 ) );
    TEST_ASSERT_EQUAL( history->queue[1].event, EVENT( Exit ) );
    TEST_ASSERT_EQUAL( history->queue[2].event, EVENT( Exit ) );
    TEST_ASSERT_EQUAL( history->queue[3].event, EVENT( Enter ) );
    TEST_ASSERT_EQUAL( history->queue[4].event, EVENT( Enter ) );
    TEST_ASSERT_EQUAL( history->queue[5].event, NULL );

    TEST_ASSERT_EQUAL( state.state, STATE( B0 ) );
}

static void test_STATE_TransitionUpAndAcross( void )
{
    STATE_UnitTestInit();
    state_t state;
    fifo_base_t * history_base = STATE_GetHistory();
    history_fifo_t * history = (history_fifo_t*)history_base;

    state.state = STATE( A0 );

    STATEMACHINE_Dispatch( &state, EVENT( TransitionToB ) );
    TEST_ASSERT_EQUAL( history->base.fill, 4U ); 
    TEST_ASSERT_EQUAL( history->queue[0].state, STATE( A0 ) );
    TEST_ASSERT_EQUAL( history->queue[1].state, STATE( A0 ) );
    TEST_ASSERT_EQUAL( history->queue[2].state, STATE( A ) );
    TEST_ASSERT_EQUAL( history->queue[3].state, STATE( B ) );
    TEST_ASSERT_EQUAL( history->queue[5].state, NULL );
    
    TEST_ASSERT_EQUAL( history->queue[0].event, EVENT( TransitionToB ) );
    TEST_ASSERT_EQUAL( history->queue[1].event, EVENT( Exit ) );
    TEST_ASSERT_EQUAL( history->queue[2].event, EVENT( Exit ) );
    TEST_ASSERT_EQUAL( history->queue[3].event, EVENT( Enter ) );
    TEST_ASSERT_EQUAL( history->queue[4].event, NULL );

    TEST_ASSERT_EQUAL( state.state, STATE( B ) );
}

static void test_STATE_TransitionAcrossAndDown( void )
{
    STATE_UnitTestInit();
    state_t state;
    fifo_base_t * history_base = STATE_GetHistory();
    history_fifo_t * history = (history_fifo_t*)history_base;

    state.state = STATE( B );

    STATEMACHINE_Dispatch( &state, EVENT( TransitionToA0 ) );
    TEST_ASSERT_EQUAL( history->base.fill, 4U ); 
    TEST_ASSERT_EQUAL( history->queue[0].state, STATE( B ) );
    TEST_ASSERT_EQUAL( history->queue[1].state, STATE( B ) );
    TEST_ASSERT_EQUAL( history->queue[2].state, STATE( A ) );
    TEST_ASSERT_EQUAL( history->queue[3].state, STATE( A0 ) );
    TEST_ASSERT_EQUAL( history->queue[5].state, NULL );
    
    TEST_ASSERT_EQUAL( history->queue[0].event, EVENT( TransitionToA0 ) );
    TEST_ASSERT_EQUAL( history->queue[1].event, EVENT( Exit ) );
    TEST_ASSERT_EQUAL( history->queue[2].event, EVENT( Enter ) );
    TEST_ASSERT_EQUAL( history->queue[3].event, EVENT( Enter ) );
    TEST_ASSERT_EQUAL( history->queue[4].event, NULL );

    TEST_ASSERT_EQUAL( state.state, STATE( A0 ) );
}

static void test_STATE_TransitionOutIntoParent( void )
{
    STATE_UnitTestInit();
    state_t state;
    fifo_base_t * history_base = STATE_GetHistory();
    history_fifo_t * history = (history_fifo_t*)history_base;

    state.state = STATE( A0 );

    STATEMACHINE_Dispatch( &state, EVENT( TransitionToA ) );
    TEST_ASSERT_EQUAL( history->base.fill, 2U ); 
    TEST_ASSERT_EQUAL( history->queue[0].state, STATE( A0 ) );
    TEST_ASSERT_EQUAL( history->queue[1].state, STATE( A0 ) );
    TEST_ASSERT_EQUAL( history->queue[2].state, NULL );
    
    TEST_ASSERT_EQUAL( history->queue[0].event, EVENT( TransitionToA ) );
    TEST_ASSERT_EQUAL( history->queue[1].event, EVENT( Exit ) );
    TEST_ASSERT_EQUAL( history->queue[2].event, NULL );

    TEST_ASSERT_EQUAL( state.state, STATE( A ) );
}

static void test_STATE_TransitionIntoItself( void )
{
    STATE_UnitTestInit();
    state_t state;
    fifo_base_t * history_base = STATE_GetHistory();
    history_fifo_t * history = (history_fifo_t*)history_base;

    state.state = STATE( A0 );

    STATEMACHINE_Dispatch( &state, EVENT( TransitionToA0 ) );
    TEST_ASSERT_EQUAL( history->base.fill, 3U ); 
    TEST_ASSERT_EQUAL( history->queue[0].state, STATE( A0 ) );
    TEST_ASSERT_EQUAL( history->queue[1].state, STATE( A0 ) );
    TEST_ASSERT_EQUAL( history->queue[2].state, STATE( A0 ) );
    TEST_ASSERT_EQUAL( history->queue[3].state, NULL );
    
    TEST_ASSERT_EQUAL( history->queue[0].event, EVENT( TransitionToA0 ) );
    TEST_ASSERT_EQUAL( history->queue[1].event, EVENT( Exit ) );
    TEST_ASSERT_EQUAL( history->queue[2].event, EVENT( Enter ) );
    TEST_ASSERT_EQUAL( history->queue[3].event, NULL );

    TEST_ASSERT_EQUAL( state.state, STATE( A0 ) );
}

static void test_STATE_TransitionWhileEntering( void )
{
    STATE_UnitTestInit();
    state_t state;
    fifo_base_t * history_base = STATE_GetHistory();
    history_fifo_t * history = (history_fifo_t*)history_base;

    state.state = STATE( A0 );

    STATEMACHINE_Dispatch( &state, EVENT( TransitionToB1 ) );
    TEST_ASSERT_EQUAL( history->base.fill, 9U ); 
    TEST_ASSERT_EQUAL( history->queue[0].state, STATE( A0 ) );
    TEST_ASSERT_EQUAL( history->queue[1].state, STATE( A0 ) );
    TEST_ASSERT_EQUAL( history->queue[2].state, STATE( A ) );
    TEST_ASSERT_EQUAL( history->queue[3].state, STATE( B ) );
    TEST_ASSERT_EQUAL( history->queue[4].state, STATE( B1 ) );
    TEST_ASSERT_EQUAL( history->queue[5].state, STATE( B1 ) );
    TEST_ASSERT_EQUAL( history->queue[6].state, STATE( B ) );
    TEST_ASSERT_EQUAL( history->queue[7].state, STATE( A ) );
    TEST_ASSERT_EQUAL( history->queue[8].state, STATE( A1 ) );
    TEST_ASSERT_EQUAL( history->queue[9].state, NULL );
    
    TEST_ASSERT_EQUAL( history->queue[0].event, EVENT( TransitionToB1 ) );
    TEST_ASSERT_EQUAL( history->queue[1].event, EVENT( Exit ) );
    TEST_ASSERT_EQUAL( history->queue[2].event, EVENT( Exit ) );
    TEST_ASSERT_EQUAL( history->queue[3].event, EVENT( Enter ) );
    TEST_ASSERT_EQUAL( history->queue[4].event, EVENT( Enter ) );
    TEST_ASSERT_EQUAL( history->queue[5].event, EVENT( Exit ) );
    TEST_ASSERT_EQUAL( history->queue[6].event, EVENT( Exit ) );
    TEST_ASSERT_EQUAL( history->queue[7].event, EVENT( Enter ) );
    TEST_ASSERT_EQUAL( history->queue[8].event, EVENT( Enter ) );
    TEST_ASSERT_EQUAL( history->queue[9].event, NULL );

    TEST_ASSERT_EQUAL( state.state, STATE( A1 ) );
}

static void test_STATE_TransitionWhileExiting( void )
{
    STATE_UnitTestInit();
    state_t state;
    fifo_base_t * history_base = STATE_GetHistory();
    history_fifo_t * history = (history_fifo_t*)history_base;

    state.state = STATE( C );

    STATEMACHINE_Dispatch( &state, EVENT( TransitionToB ) );
    TEST_ASSERT_EQUAL( history->queue[0].state, STATE( C ) );
    TEST_ASSERT_EQUAL( history->queue[1].state, STATE( C ) );
    TEST_ASSERT_EQUAL( history->queue[2].state, STATE( A ) );
    TEST_ASSERT_EQUAL( history->queue[3].state, STATE( A0 ) );
    TEST_ASSERT_EQUAL( history->queue[4].state, NULL );
    TEST_ASSERT_EQUAL( history->base.fill, 4U ); 
    
    TEST_ASSERT_EQUAL( history->queue[0].event, EVENT( TransitionToB ) );
    TEST_ASSERT_EQUAL( history->queue[1].event, EVENT( Exit ) );
    TEST_ASSERT_EQUAL( history->queue[2].event, EVENT( Enter ) );
    TEST_ASSERT_EQUAL( history->queue[3].event, EVENT( Enter ) );
    TEST_ASSERT_EQUAL( history->queue[4].event, NULL );

    TEST_ASSERT_EQUAL( state.state, STATE( A0 ) );
}


extern void STATETestSuite(void)
{
    RUN_TEST( test_STATE_Preprocessor );

    RUN_TEST( test_STATE_FIFOInit );
    RUN_TEST( test_STATE_FIFOAddRemoveEvent );
    RUN_TEST( test_STATE_FIFOWrapAround );
    RUN_TEST( test_STATE_FIFOFlush );

    RUN_TEST( test_STATE_Init );
    RUN_TEST( test_STATE_SingleEvent );
    RUN_TEST( test_STATE_SingleUnhandledEvent );
    RUN_TEST( test_STATE_TransitionSharedParent );
    RUN_TEST( test_STATE_TransitionNoSharedParent );
    RUN_TEST( test_STATE_TransitionUpAndAcross );
    RUN_TEST( test_STATE_TransitionAcrossAndDown );
    RUN_TEST( test_STATE_TransitionOutIntoParent );
    RUN_TEST( test_STATE_TransitionIntoItself );
    RUN_TEST( test_STATE_TransitionWhileEntering );
    RUN_TEST( test_STATE_TransitionWhileExiting );

}
