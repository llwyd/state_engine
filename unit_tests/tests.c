#include "unity.h"
#include "../src/fsm.h"

#define SIGNALS(SIG) \
  SIG( TransitionToA ) \
  SIG( TransitionToB ) \
  SIG( TransitionToA0 ) \
  SIG( TransitionToA1 ) \
  SIG( TransitionToB0 ) \

#define STATES(ST) \
  ST( A ) \
  ST( B ) \
  ST( A0 ) \
  ST( A1 ) \
  ST( B0 ) \

GENERATE_SIGNALS( SIGNALS );
GENERATE_SIGNAL_STRINGS( SIGNALS );
GENERATE_STATE_PROTOTYPES( STATES );

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

void setUp( void )
{

}

void tearDown( void )
{

}

void test_STATE_Preprocessor( void )
{
    TEST_ASSERT_EQUAL( EVENT(Tick), EVENT(DefaultCount));
}

void test_FIFO_Init( void )
{
    state_fifo_t events;
    STATE_InitEventBuffer( &events );

    TEST_ASSERT_EQUAL( events.read_index, 0U );
    TEST_ASSERT_EQUAL( events.write_index, 0U );
    TEST_ASSERT_EQUAL( events.fill, 0U );
}

void test_FIFO_AddRemoveEvent( void )
{
    state_fifo_t events;
    STATE_InitEventBuffer( &events );

    TEST_ASSERT_EQUAL( events.read_index, 0U );
    TEST_ASSERT_EQUAL( events.write_index, 0U );
    TEST_ASSERT_EQUAL( events.fill, 0U );
    TEST_ASSERT_FALSE( FSM_EventsAvailable( &events ) );

    FSM_AddEvent( &events, EVENT( Tick ) );
    TEST_ASSERT_EQUAL( events.read_index, 0U );
    TEST_ASSERT_EQUAL( events.write_index, 1U );
    TEST_ASSERT_EQUAL( events.fill, 1U );

    TEST_ASSERT_TRUE( FSM_EventsAvailable( &events ) );

    event_t event = FSM_GetLatestEvent( &events );
    TEST_ASSERT_EQUAL( event, EVENT( Tick ) );
    TEST_ASSERT_EQUAL( events.read_index, 1U );
    TEST_ASSERT_EQUAL( events.write_index, 1U );
    TEST_ASSERT_EQUAL( events.fill, 0U );
}

void test_FIFO_WrapAround( void )
{
    state_fifo_t events;
    events.write_index = ( FIFO_BUFFER_SIZE - 1U );
    events.read_index = ( FIFO_BUFFER_SIZE - 1U );
    events.fill = 0;


    FSM_AddEvent( &events, EVENT( Tick ) );
    TEST_ASSERT_EQUAL( events.read_index, (FIFO_BUFFER_SIZE - 1U ));
    TEST_ASSERT_EQUAL( events.write_index, 0U );
    TEST_ASSERT_EQUAL( events.fill, 1U );
}

void test_FIFO_Flush( void )
{
    state_fifo_t events;
    STATE_InitEventBuffer( &events );
    
    FSM_AddEvent( &events, EVENT( Tick ) );
    FSM_AddEvent( &events, EVENT( Tick ) );
    FSM_AddEvent( &events, EVENT( Tick ) );
    FSM_AddEvent( &events, EVENT( Tick ) );

    TEST_ASSERT_EQUAL( events.read_index, 0U );
    TEST_ASSERT_EQUAL( events.write_index, 4U );
    TEST_ASSERT_EQUAL( events.fill, 4U );
    TEST_ASSERT_TRUE( FSM_EventsAvailable( &events ) );

    FSM_FlushEvents( &events );
    TEST_ASSERT_EQUAL( events.read_index, 4U );
    TEST_ASSERT_EQUAL( events.write_index, 4U );
    TEST_ASSERT_EQUAL( events.fill, 0U );
    TEST_ASSERT_FALSE( FSM_EventsAvailable( &events ) );
}

void test_STATE_Init( void )
{
    STATE_UnitTestInit();

    state_t state;
    state_fifo_t events;

    state_history_t * history = STATE_GetHistory();

    /* State machine should
     * StateA -> Enter
     * StateA0 ->Enter
     */

    FSM_Init( &state, &events , STATE( A0 ) );
    
    TEST_ASSERT_EQUAL( history->fill, 2U ); 
    TEST_ASSERT_EQUAL( history->data[0].state, STATE( A ) );
    TEST_ASSERT_EQUAL( history->data[1].state, STATE( A0 ) );

    TEST_ASSERT_EQUAL( history->data[0].event, EVENT( Enter ) );
    TEST_ASSERT_EQUAL( history->data[1].event, EVENT( Enter ) );
    
    TEST_ASSERT_EQUAL( history->data[2].state, NULL );
    TEST_ASSERT_EQUAL( history->data[2].event, NULL );
}

void test_STATE_SingleEvent( void )
{
    STATE_UnitTestInit();
    state_t state;
    state_history_t * history = STATE_GetHistory();

    state.state = STATE( A );

    FSM_HierarchicalDispatch( &state, EVENT( Tick ) );
    TEST_ASSERT_EQUAL( history->fill, 1U ); 
    TEST_ASSERT_EQUAL( history->data[0].state, STATE( A ) );
    TEST_ASSERT_EQUAL( history->data[1].state, NULL );
    TEST_ASSERT_EQUAL( history->data[0].event, EVENT( Tick ) );
    TEST_ASSERT_EQUAL( history->data[1].event, NULL );

    TEST_ASSERT_EQUAL( state.state, STATE( A ) );
}

void test_STATE_SingleUnhandledEvent( void )
{
    STATE_UnitTestInit();
    state_t state;
    state_history_t * history = STATE_GetHistory();

    state.state = STATE( A0 );

    FSM_HierarchicalDispatch( &state, EVENT( Tick ) );
    TEST_ASSERT_EQUAL( history->fill, 2U ); 
    TEST_ASSERT_EQUAL( history->data[0].state, STATE( A0 ) );
    TEST_ASSERT_EQUAL( history->data[1].state, STATE ( A ) );
    TEST_ASSERT_EQUAL( history->data[2].state, NULL );
    
    TEST_ASSERT_EQUAL( history->data[0].event, EVENT( Tick ) );
    TEST_ASSERT_EQUAL( history->data[1].event, EVENT( Tick ) );
    TEST_ASSERT_EQUAL( history->data[2].event, NULL );

    TEST_ASSERT_EQUAL( state.state, STATE( A0 ) );
}

int main( void )
{
    UNITY_BEGIN();

    RUN_TEST( test_STATE_Preprocessor );

    RUN_TEST( test_FIFO_Init );
    RUN_TEST( test_FIFO_AddRemoveEvent );
    RUN_TEST( test_FIFO_WrapAround );
    RUN_TEST( test_FIFO_Flush );

    RUN_TEST( test_STATE_Init );
    RUN_TEST( test_STATE_SingleEvent );
    RUN_TEST( test_STATE_SingleUnhandledEvent );

    return UNITY_END();
}
