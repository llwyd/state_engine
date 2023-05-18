#include "state_tests.h"
#include "state.h"
#include "unity.h"
#include "fifo.h"

#define SIGNALS(SIG) \
  SIG( TransitionToA ) \
  SIG( TransitionToB ) \
  SIG( TransitionToA0 ) \
  SIG( TransitionToA1 ) \
  SIG( TransitionToB0 ) \
  SIG( TransitionToB1 ) \

#define STATES(ST) \
  ST( A ) \
  ST( B ) \
  ST( A0 ) \
  ST( A1 ) \
  ST( B0 ) \
  ST( B1 ) \
  ST( C ) \

GENERATE_SIGNALS( SIGNALS );
GENERATE_STATE_PROTOTYPES( STATES );

CREATE_FIFO(_StateEvent, event_fifo_t, event_t, 32U );

static state_ret_t State_A( state_t * this, event_t s)
{
  state_ret_t ret;

  switch( s )
  {
    case EVENT(Enter):
      HANDLED();
      break;
    case EVENT(Exit):
      HANDLED();
      break;
    case EVENT(Tick):
      HANDLED();
      break;
    case EVENT(TransitionToB):
      TRANSITION( B );
      break;
    default:
      NO_PARENT();
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
      HANDLED();
      break;
    case EVENT(Exit):
      HANDLED();
      break;
    case EVENT(Tick):
      HANDLED();
      break;
    case EVENT(TransitionToA0):
      TRANSITION( A0 );
      break;
    default:
      NO_PARENT();
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
      HANDLED();
      break;
    case EVENT(Exit):
      HANDLED();
      break;
    case EVENT(TransitionToA):
      TRANSITION( A );
      break;
    case EVENT(TransitionToA1):
      TRANSITION( A1 );
      break;
    case EVENT(TransitionToB):
      TRANSITION( B );
      break;
    case EVENT(TransitionToB0):
      TRANSITION( B0 );
      break;
    case EVENT(TransitionToA0):
      TRANSITION( A0 );
      break;
    case EVENT(TransitionToB1):
      TRANSITION( B1 );
      break;
    default:
      PARENT( A );
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
      HANDLED();
      break;
    case EVENT(Exit):
      HANDLED();
      break;
    case EVENT(TransitionToA0):
      TRANSITION( A0 );
      break;
    default:
      PARENT( A );
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
      HANDLED();
      break;
    case EVENT(Exit):
      HANDLED();
      break;
    case EVENT(TransitionToA0):
      TRANSITION( A0 );
      break;
    default:
      PARENT( B );
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
      TRANSITION( A1 );
      break;
    case EVENT(Exit):
      HANDLED();
      break;
    case EVENT(TransitionToA0):
      TRANSITION( A0 );
      break;
    default:
      PARENT( B );
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
      HANDLED();
      break;
    case EVENT(Exit):
      TRANSITION( A0 );
      break;
    case EVENT(Tick):
      HANDLED();
      break;
    case EVENT(TransitionToB):
      TRANSITION( B );
      break;
    default:
      NO_PARENT();
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
    TEST_ASSERT_EQUAL( EVENT(Tick), EVENT(DefaultCount));
}

static void test_FIFO_Init( void )
{
    event_fifo_t events;
    FIFO_Init_StateEvent( &events );

    TEST_ASSERT_EQUAL( events.base.r_index, 0U );
    TEST_ASSERT_EQUAL( events.base.w_index, 0U );
    TEST_ASSERT_EQUAL( events.base.fill, 0U );
}

static void test_FIFO_AddRemoveEvent( void )
{
    event_fifo_t events;
    FIFO_Init_StateEvent( &events );

    TEST_ASSERT_EQUAL( events.base.r_index, 0U );
    TEST_ASSERT_EQUAL( events.base.w_index, 0U );
    TEST_ASSERT_EQUAL( events.base.fill, 0U );
    TEST_ASSERT_FALSE( FIFO_NE_StateEvent( &events ) );

    FIFO_ENQ_StateEvent( &events, EVENT( Tick ) );
    TEST_ASSERT_EQUAL( events.base.r_index, 0U );
    TEST_ASSERT_EQUAL( events.base.w_index, 1U );
    TEST_ASSERT_EQUAL( events.base.fill, 1U );

    TEST_ASSERT_TRUE( FIFO_NE_StateEvent( &events ) );

    event_t event = FIFO_DEQ_StateEvent( &events );
    TEST_ASSERT_EQUAL( event, EVENT( Tick ) );
    TEST_ASSERT_EQUAL( events.base.r_index, 1U );
    TEST_ASSERT_EQUAL( events.base.w_index, 1U );
    TEST_ASSERT_EQUAL( events.base.fill, 0U );
}

static void test_FIFO_WrapAround( void )
{
    event_fifo_t events;
    FIFO_Init_StateEvent( &events );
    
    events.base.w_index = ( events.base.max - 1U );
    events.base.r_index = ( events.base.max - 1U );
    events.base.fill = 0;

    FIFO_ENQ_StateEvent( &events, EVENT( Tick ) );
    TEST_ASSERT_EQUAL( events.base.r_index, (events.base.max - 1U ));
    TEST_ASSERT_EQUAL( events.base.w_index, 0U );
    TEST_ASSERT_EQUAL( events.base.fill, 1U );
}

static void test_FIFO_Flush( void )
{
    event_fifo_t events;
    FIFO_Init_StateEvent( &events );
    
    FIFO_ENQ_StateEvent( &events, EVENT( Tick ) );
    FIFO_ENQ_StateEvent( &events, EVENT( Tick ) );
    FIFO_ENQ_StateEvent( &events, EVENT( Tick ) );
    FIFO_ENQ_StateEvent( &events, EVENT( Tick ) );

    TEST_ASSERT_EQUAL( events.base.r_index, 0U );
    TEST_ASSERT_EQUAL( events.base.w_index, 4U );
    TEST_ASSERT_EQUAL( events.base.fill, 4U );
    TEST_ASSERT_TRUE( FIFO_NE_StateEvent( &events ) );

    FIFO_FLUSH_StateEvent( &events );
    TEST_ASSERT_EQUAL( events.base.r_index, 4U );
    TEST_ASSERT_EQUAL( events.base.w_index, 4U );
    TEST_ASSERT_EQUAL( events.base.fill, 0U );
    TEST_ASSERT_FALSE( FIFO_NE_StateEvent( &events ) );
}

static void test_STATE_Init( void )
{
    STATE_UnitTestInit();

    state_t state;
    event_fifo_t events;

    state_history_t * history = STATE_GetHistory();

    /* State machine should
     * StateA -> Enter
     * StateA0 ->Enter
     */

    FIFO_Init_StateEvent( &events );
    STATEMACHINE_Init( &state, STATE( A0 ) );
    
    TEST_ASSERT_EQUAL( history->fill, 2U ); 
    TEST_ASSERT_EQUAL( history->data[0].state, STATE( A ) );
    TEST_ASSERT_EQUAL( history->data[1].state, STATE( A0 ) );

    TEST_ASSERT_EQUAL( history->data[0].event, EVENT( Enter ) );
    TEST_ASSERT_EQUAL( history->data[1].event, EVENT( Enter ) );
    
    TEST_ASSERT_EQUAL( history->data[2].state, NULL );
    TEST_ASSERT_EQUAL( history->data[2].event, NULL );
}

static void test_STATE_SingleEvent( void )
{
    STATE_UnitTestInit();
    state_t state;
    state_history_t * history = STATE_GetHistory();

    state.state = STATE( A );

    STATEMACHINE_Dispatch( &state, EVENT( Tick ) );
    TEST_ASSERT_EQUAL( history->fill, 1U ); 
    TEST_ASSERT_EQUAL( history->data[0].state, STATE( A ) );
    TEST_ASSERT_EQUAL( history->data[1].state, NULL );
    TEST_ASSERT_EQUAL( history->data[0].event, EVENT( Tick ) );
    TEST_ASSERT_EQUAL( history->data[1].event, NULL );

    TEST_ASSERT_EQUAL( state.state, STATE( A ) );
}

static void test_STATE_SingleUnhandledEvent( void )
{
    STATE_UnitTestInit();
    state_t state;
    state_history_t * history = STATE_GetHistory();

    state.state = STATE( A0 );

    STATEMACHINE_Dispatch( &state, EVENT( Tick ) );
    TEST_ASSERT_EQUAL( history->fill, 2U ); 
    TEST_ASSERT_EQUAL( history->data[0].state, STATE( A0 ) );
    TEST_ASSERT_EQUAL( history->data[1].state, STATE ( A ) );
    TEST_ASSERT_EQUAL( history->data[2].state, NULL );
    
    TEST_ASSERT_EQUAL( history->data[0].event, EVENT( Tick ) );
    TEST_ASSERT_EQUAL( history->data[1].event, EVENT( Tick ) );
    TEST_ASSERT_EQUAL( history->data[2].event, NULL );

    TEST_ASSERT_EQUAL( state.state, STATE( A0 ) );
}

static void test_STATE_TransitionSharedParent( void )
{
    STATE_UnitTestInit();
    state_t state;
    state_history_t * history = STATE_GetHistory();

    state.state = STATE( A0 );

    STATEMACHINE_Dispatch( &state, EVENT( TransitionToA1 ) );
    TEST_ASSERT_EQUAL( history->fill, 3U ); 
    TEST_ASSERT_EQUAL( history->data[0].state, STATE( A0 ) );
    TEST_ASSERT_EQUAL( history->data[1].state, STATE( A0 ) );
    TEST_ASSERT_EQUAL( history->data[2].state, STATE( A1 ) );
    TEST_ASSERT_EQUAL( history->data[3].state, NULL );
    
    TEST_ASSERT_EQUAL( history->data[0].event, EVENT( TransitionToA1 ) );
    TEST_ASSERT_EQUAL( history->data[1].event, EVENT( Exit ) );
    TEST_ASSERT_EQUAL( history->data[2].event, EVENT( Enter ) );
    TEST_ASSERT_EQUAL( history->data[3].event, NULL );

    TEST_ASSERT_EQUAL( state.state, STATE( A1 ) );
    
    STATE_UnitTestInit();
    
    state.state = STATE( A );

    STATEMACHINE_Dispatch( &state, EVENT( TransitionToB ) );
    TEST_ASSERT_EQUAL( history->fill, 3U ); 
    TEST_ASSERT_EQUAL( history->data[0].state, STATE( A ) );
    TEST_ASSERT_EQUAL( history->data[1].state, STATE( A ) );
    TEST_ASSERT_EQUAL( history->data[2].state, STATE( B ) );
    TEST_ASSERT_EQUAL( history->data[3].state, NULL );
    
    TEST_ASSERT_EQUAL( history->data[0].event, EVENT( TransitionToB ) );
    TEST_ASSERT_EQUAL( history->data[1].event, EVENT( Exit ) );
    TEST_ASSERT_EQUAL( history->data[2].event, EVENT( Enter ) );
    TEST_ASSERT_EQUAL( history->data[3].event, NULL );

    TEST_ASSERT_EQUAL( state.state, STATE( B ) );
}

static void test_STATE_TransitionNoSharedParent( void )
{
    STATE_UnitTestInit();
    state_t state;
    state_history_t * history = STATE_GetHistory();

    state.state = STATE( A0 );

    STATEMACHINE_Dispatch( &state, EVENT( TransitionToB0 ) );
    TEST_ASSERT_EQUAL( history->fill, 5U ); 
    TEST_ASSERT_EQUAL( history->data[0].state, STATE( A0 ) );
    TEST_ASSERT_EQUAL( history->data[1].state, STATE( A0 ) );
    TEST_ASSERT_EQUAL( history->data[2].state, STATE( A ) );
    TEST_ASSERT_EQUAL( history->data[3].state, STATE( B ) );
    TEST_ASSERT_EQUAL( history->data[4].state, STATE( B0 ) );
    TEST_ASSERT_EQUAL( history->data[5].state, NULL );
    
    TEST_ASSERT_EQUAL( history->data[0].event, EVENT( TransitionToB0 ) );
    TEST_ASSERT_EQUAL( history->data[1].event, EVENT( Exit ) );
    TEST_ASSERT_EQUAL( history->data[2].event, EVENT( Exit ) );
    TEST_ASSERT_EQUAL( history->data[3].event, EVENT( Enter ) );
    TEST_ASSERT_EQUAL( history->data[4].event, EVENT( Enter ) );
    TEST_ASSERT_EQUAL( history->data[5].event, NULL );

    TEST_ASSERT_EQUAL( state.state, STATE( B0 ) );
}

static void test_STATE_TransitionUpAndAcross( void )
{
    STATE_UnitTestInit();
    state_t state;
    state_history_t * history = STATE_GetHistory();

    state.state = STATE( A0 );

    STATEMACHINE_Dispatch( &state, EVENT( TransitionToB ) );
    TEST_ASSERT_EQUAL( history->fill, 4U ); 
    TEST_ASSERT_EQUAL( history->data[0].state, STATE( A0 ) );
    TEST_ASSERT_EQUAL( history->data[1].state, STATE( A0 ) );
    TEST_ASSERT_EQUAL( history->data[2].state, STATE( A ) );
    TEST_ASSERT_EQUAL( history->data[3].state, STATE( B ) );
    TEST_ASSERT_EQUAL( history->data[5].state, NULL );
    
    TEST_ASSERT_EQUAL( history->data[0].event, EVENT( TransitionToB ) );
    TEST_ASSERT_EQUAL( history->data[1].event, EVENT( Exit ) );
    TEST_ASSERT_EQUAL( history->data[2].event, EVENT( Exit ) );
    TEST_ASSERT_EQUAL( history->data[3].event, EVENT( Enter ) );
    TEST_ASSERT_EQUAL( history->data[4].event, NULL );

    TEST_ASSERT_EQUAL( state.state, STATE( B ) );
}

static void test_STATE_TransitionAcrossAndDown( void )
{
    STATE_UnitTestInit();
    state_t state;
    state_history_t * history = STATE_GetHistory();

    state.state = STATE( B );

    STATEMACHINE_Dispatch( &state, EVENT( TransitionToA0 ) );
    TEST_ASSERT_EQUAL( history->fill, 4U ); 
    TEST_ASSERT_EQUAL( history->data[0].state, STATE( B ) );
    TEST_ASSERT_EQUAL( history->data[1].state, STATE( B ) );
    TEST_ASSERT_EQUAL( history->data[2].state, STATE( A ) );
    TEST_ASSERT_EQUAL( history->data[3].state, STATE( A0 ) );
    TEST_ASSERT_EQUAL( history->data[5].state, NULL );
    
    TEST_ASSERT_EQUAL( history->data[0].event, EVENT( TransitionToA0 ) );
    TEST_ASSERT_EQUAL( history->data[1].event, EVENT( Exit ) );
    TEST_ASSERT_EQUAL( history->data[2].event, EVENT( Enter ) );
    TEST_ASSERT_EQUAL( history->data[3].event, EVENT( Enter ) );
    TEST_ASSERT_EQUAL( history->data[4].event, NULL );

    TEST_ASSERT_EQUAL( state.state, STATE( A0 ) );
}

static void test_STATE_TransitionOutIntoParent( void )
{
    STATE_UnitTestInit();
    state_t state;
    state_history_t * history = STATE_GetHistory();

    state.state = STATE( A0 );

    STATEMACHINE_Dispatch( &state, EVENT( TransitionToA ) );
    TEST_ASSERT_EQUAL( history->fill, 2U ); 
    TEST_ASSERT_EQUAL( history->data[0].state, STATE( A0 ) );
    TEST_ASSERT_EQUAL( history->data[1].state, STATE( A0 ) );
    TEST_ASSERT_EQUAL( history->data[2].state, NULL );
    
    TEST_ASSERT_EQUAL( history->data[0].event, EVENT( TransitionToA ) );
    TEST_ASSERT_EQUAL( history->data[1].event, EVENT( Exit ) );
    TEST_ASSERT_EQUAL( history->data[2].event, NULL );

    TEST_ASSERT_EQUAL( state.state, STATE( A ) );
}

static void test_STATE_TransitionIntoItself( void )
{
    STATE_UnitTestInit();
    state_t state;
    state_history_t * history = STATE_GetHistory();

    state.state = STATE( A0 );

    STATEMACHINE_Dispatch( &state, EVENT( TransitionToA0 ) );
    TEST_ASSERT_EQUAL( history->fill, 3U ); 
    TEST_ASSERT_EQUAL( history->data[0].state, STATE( A0 ) );
    TEST_ASSERT_EQUAL( history->data[1].state, STATE( A0 ) );
    TEST_ASSERT_EQUAL( history->data[2].state, STATE( A0 ) );
    TEST_ASSERT_EQUAL( history->data[3].state, NULL );
    
    TEST_ASSERT_EQUAL( history->data[0].event, EVENT( TransitionToA0 ) );
    TEST_ASSERT_EQUAL( history->data[1].event, EVENT( Exit ) );
    TEST_ASSERT_EQUAL( history->data[2].event, EVENT( Enter ) );
    TEST_ASSERT_EQUAL( history->data[3].event, NULL );

    TEST_ASSERT_EQUAL( state.state, STATE( A0 ) );
}

static void test_STATE_TransitionWhileEntering( void )
{
    STATE_UnitTestInit();
    state_t state;
    state_history_t * history = STATE_GetHistory();

    state.state = STATE( A0 );

    STATEMACHINE_Dispatch( &state, EVENT( TransitionToB1 ) );
    TEST_ASSERT_EQUAL( history->fill, 9U ); 
    TEST_ASSERT_EQUAL( history->data[0].state, STATE( A0 ) );
    TEST_ASSERT_EQUAL( history->data[1].state, STATE( A0 ) );
    TEST_ASSERT_EQUAL( history->data[2].state, STATE( A ) );
    TEST_ASSERT_EQUAL( history->data[3].state, STATE( B ) );
    TEST_ASSERT_EQUAL( history->data[4].state, STATE( B1 ) );
    TEST_ASSERT_EQUAL( history->data[5].state, STATE( B1 ) );
    TEST_ASSERT_EQUAL( history->data[6].state, STATE( B ) );
    TEST_ASSERT_EQUAL( history->data[7].state, STATE( A ) );
    TEST_ASSERT_EQUAL( history->data[8].state, STATE( A1 ) );
    TEST_ASSERT_EQUAL( history->data[9].state, NULL );
    
    TEST_ASSERT_EQUAL( history->data[0].event, EVENT( TransitionToB1 ) );
    TEST_ASSERT_EQUAL( history->data[1].event, EVENT( Exit ) );
    TEST_ASSERT_EQUAL( history->data[2].event, EVENT( Exit ) );
    TEST_ASSERT_EQUAL( history->data[3].event, EVENT( Enter ) );
    TEST_ASSERT_EQUAL( history->data[4].event, EVENT( Enter ) );
    TEST_ASSERT_EQUAL( history->data[5].event, EVENT( Exit ) );
    TEST_ASSERT_EQUAL( history->data[6].event, EVENT( Exit ) );
    TEST_ASSERT_EQUAL( history->data[7].event, EVENT( Enter ) );
    TEST_ASSERT_EQUAL( history->data[8].event, EVENT( Enter ) );
    TEST_ASSERT_EQUAL( history->data[9].event, NULL );

    TEST_ASSERT_EQUAL( state.state, STATE( A1 ) );
}

static void test_STATE_TransitionWhileExiting( void )
{
    STATE_UnitTestInit();
    state_t state;
    state_history_t * history = STATE_GetHistory();

    state.state = STATE( C );

    STATEMACHINE_Dispatch( &state, EVENT( TransitionToB ) );
    TEST_ASSERT_EQUAL( history->data[0].state, STATE( C ) );
    TEST_ASSERT_EQUAL( history->data[1].state, STATE( C ) );
    TEST_ASSERT_EQUAL( history->data[2].state, STATE( A ) );
    TEST_ASSERT_EQUAL( history->data[3].state, STATE( A0 ) );
    TEST_ASSERT_EQUAL( history->data[4].state, NULL );
    TEST_ASSERT_EQUAL( history->fill, 4U ); 
    
    TEST_ASSERT_EQUAL( history->data[0].event, EVENT( TransitionToB ) );
    TEST_ASSERT_EQUAL( history->data[1].event, EVENT( Exit ) );
    TEST_ASSERT_EQUAL( history->data[2].event, EVENT( Enter ) );
    TEST_ASSERT_EQUAL( history->data[3].event, EVENT( Enter ) );
    TEST_ASSERT_EQUAL( history->data[4].event, NULL );

    TEST_ASSERT_EQUAL( state.state, STATE( A0 ) );
}


extern void STATETestSuite(void)
{
    RUN_TEST( test_STATE_Preprocessor );

    RUN_TEST( test_FIFO_Init );
    RUN_TEST( test_FIFO_AddRemoveEvent );
    RUN_TEST( test_FIFO_WrapAround );
    RUN_TEST( test_FIFO_Flush );

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
