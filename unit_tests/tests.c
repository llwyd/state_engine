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

int main( void )
{
    UNITY_BEGIN();

    RUN_TEST( test_STATE_Preprocessor );
    RUN_TEST( test_FIFO_Init );
    RUN_TEST( test_FIFO_AddRemoveEvent );
    RUN_TEST( test_FIFO_WrapAround );
    RUN_TEST( test_FIFO_Flush );

    return UNITY_END();
}
