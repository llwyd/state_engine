#include "state_tests.h"
#include "state.h"
#include "events.h"
#include "unity.h"
#include <string.h>

#define SIGNALS(SIG) \
    SIG(Tick) \
    SIG(TestEvent0) \
    SIG(TestEvent1) \
    SIG( TransitionToA ) \
    SIG( TransitionToB ) \
    SIG( TransitionToA0 ) \
    SIG( TransitionToA1 ) \
    SIG( TransitionToB0 ) \
    SIG( TransitionToB1 ) \
    SIG( TransitionToA01 )  

GENERATE_SIGNALS( SIGNALS );
GENERATE_SIGNAL_STRINGS( SIGNALS );

DEFINE_STATE(A);
DEFINE_STATE(B);
DEFINE_STATE(A0);
DEFINE_STATE(A00);
DEFINE_STATE(A01);
DEFINE_STATE(A1);
DEFINE_STATE(B0);
DEFINE_STATE(B1);
DEFINE_STATE(C);

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
      ret = TRANSITION( this, STATE(B) );
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
      ret = TRANSITION( this, STATE(A0) );
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
      ret = TRANSITION( this, STATE(A) );
      break;
    case EVENT(TransitionToA1):
      ret = TRANSITION( this, STATE(A1) );
      break;
    case EVENT(TransitionToB):
      ret = TRANSITION( this, STATE(B) );
      break;
    case EVENT(TransitionToB0):
      ret = TRANSITION( this, STATE(B0) );
      break;
    case EVENT(TransitionToA0):
      ret = TRANSITION( this, STATE(A0) );
      break;
    case EVENT(TransitionToB1):
      ret = TRANSITION( this, STATE(B1) );
      break;
    default:
      ret = PARENT( this, STATE(A) );
      break;
  }

  return ret;
}

static state_ret_t State_A00( state_t * this, event_t s )
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
    case EVENT(TransitionToA1):
      ret = TRANSITION( this, STATE(A1) );
      break;
    case EVENT(TransitionToA01):
      ret = TRANSITION( this, STATE(A01) );
      break;
    default:
      ret = PARENT( this, STATE(A0) );
      break;
  }

  return ret;
}

static state_ret_t State_A01( state_t * this, event_t s )
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
    default:
      ret = PARENT( this, STATE(A0) );
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
      ret = TRANSITION( this, STATE(A0) );
      break;
    default:
      ret = PARENT( this, STATE(A) );
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
      ret = TRANSITION( this, STATE(A0) );
      break;
    default:
      ret = PARENT( this, STATE(B) );
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
      ret = TRANSITION( this, STATE(A1) );
      break;
    case EVENT(Exit):
      ret = HANDLED(this);
      break;
    case EVENT(TransitionToA0):
      ret = TRANSITION( this, STATE(A0) );
      break;
    default:
      ret = PARENT( this, STATE(B) );
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
      ret = TRANSITION( this, STATE(A0) );
      break;
    case EVENT(Tick):
      ret = HANDLED(this);
      break;
    case EVENT(TransitionToB):
      ret = TRANSITION( this, STATE(B) );
      break;
    default:
      ret = NO_PARENT(this);
      break;
  }

  return ret;
}

static void test_MSTATE_FIFOInit(void)
{
    event_fifo_t events;
    Events_Init( &events );

    TEST_ASSERT_EQUAL( events.base.read_index, 0U );
    TEST_ASSERT_EQUAL( events.base.write_index, 0U );
    TEST_ASSERT_EQUAL( events.base.fill, 0U );
    
    TEST_ASSERT_EQUAL( events.in.state, NULL );
    TEST_ASSERT_EQUAL( events.in.event, 0 );
    
    TEST_ASSERT_EQUAL( events.out.state, NULL );
    TEST_ASSERT_EQUAL( events.out.event, 0 );
}

static void test_MSTATE_AddRemoveEvent(void)
{
    event_fifo_t events;
    Events_Init( &events );
    
    TEST_ASSERT_EQUAL( events.base.read_index, 0U );
    TEST_ASSERT_EQUAL( events.base.write_index, 0U );
    TEST_ASSERT_EQUAL( events.base.fill, 0U );
    TEST_ASSERT_TRUE( FIFO_IsEmpty( &events.base ) );
    
    STATE_UnitTestInit();
    state_t state;
    
    fifo_base_t * history_base = STATE_GetHistory();
    history_fifo_t * history = (history_fifo_t*)history_base;
    
    STATEMACHINE_Init( &state, STATE( A0 ) );
    
    TEST_ASSERT_EQUAL( state.state, STATE( A0 ) );
    TEST_ASSERT_EQUAL( history->base.fill, 2U ); 
    TEST_ASSERT_EQUAL( history->queue[0].state, STATE( A ) );
    TEST_ASSERT_EQUAL( history->queue[1].state, STATE( A0 ) );

    TEST_ASSERT_EQUAL( history->queue[0].event, EVENT( Enter ) );
    TEST_ASSERT_EQUAL( history->queue[1].event, EVENT( Enter ) );
    
    TEST_ASSERT_EQUAL( history->queue[2].state, NULL );
    TEST_ASSERT_EQUAL( history->queue[2].event, NULL );


    Events_Enqueue( &events, &state, EVENT( Tick ) );
    
    TEST_ASSERT_EQUAL( events.base.read_index, 0U );
    TEST_ASSERT_EQUAL( events.base.write_index, 1U );
    TEST_ASSERT_EQUAL( events.base.fill, 1U );
    TEST_ASSERT_FALSE( FIFO_IsEmpty( &events.base ) );

    state_event_t latest_event = FIFO_Dequeue(&events);
    TEST_ASSERT_EQUAL( latest_event.state, &state );
    TEST_ASSERT_EQUAL( latest_event.event, EVENT( Tick ) );
    
    TEST_ASSERT_EQUAL( events.base.read_index, 1U );
    TEST_ASSERT_EQUAL( events.base.write_index, 1U );
    TEST_ASSERT_EQUAL( events.base.fill, 0U );
    TEST_ASSERT_TRUE( FIFO_IsEmpty( &events.base ) );
}

extern void MULTISTATETestSuite(void)
{
    RUN_TEST( test_MSTATE_FIFOInit );
    RUN_TEST( test_MSTATE_AddRemoveEvent );
}
