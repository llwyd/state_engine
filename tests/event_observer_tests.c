#include "state.h"
#include "event_observer.h"
#include "unity.h"
#include <string.h>

#define SIGNALS(SIG) \
    SIG(TestEvent0) \
    SIG(TestEvent1) \
    SIG(TestEvent2) \

GENERATE_SIGNALS( SIGNALS );

DEFINE_STATE(A);

static state_ret_t State_A( state_t * this, event_t s)
{
  state_ret_t ret;

  switch( s )
  {
    case EVENT(Enter):
    case EVENT(Exit):
    case EVENT(TestEvent0):
    case EVENT(TestEvent1):
    case EVENT(TestEvent2):
      ret = HANDLED(this);
      break;
    default:
      ret = NO_PARENT(this);
      break;
  }

  return ret;
}

void test_EVENTOBS_Init(void)
{
    GENERATE_EVENT_OBSERVERS( observer, SIGNALS );

    EventObserver_Init(observer, EVENT(EventCount)); 
    for(uint32_t idx = 0; idx < EVENT(EventCount); idx++)
    {
        TEST_ASSERT_EQUAL( 0U, observer[idx].subscriptions);
        for(uint32_t jdx = 0; jdx < MAX_SUBSCRIPTIONS; jdx++)
        {
            TEST_ASSERT_EQUAL( NULL, observer[idx].subscriber[jdx]);
        }
    }
}

void test_EVENTOBS_Subscribe(void)
{
    GENERATE_EVENT_OBSERVERS( observer, SIGNALS );
    
    state_t state;
    state_t state0;
    
    STATEMACHINE_Init( &state, STATE( A ) );
    STATEMACHINE_Init( &state0, STATE( A ) );

    TEST_ASSERT_NOT_EQUAL(&state, &state0);

    EventObserver_Init(observer, EVENT(EventCount));
    EventObserver_Subscribe(observer, EVENT(TestEvent0), &state);

    uint32_t idx = (uint32_t)EVENT(TestEvent0);
    TEST_ASSERT_EQUAL(1U, observer[idx].subscriptions);
    TEST_ASSERT_EQUAL(&state, observer[idx].subscriber[0]);
    TEST_ASSERT_EQUAL(NULL, observer[idx].subscriber[1]);
    TEST_ASSERT_EQUAL(NULL, observer[idx].subscriber[2]);
    TEST_ASSERT_EQUAL(NULL, observer[idx].subscriber[3]);
    
    EventObserver_Subscribe(observer, EVENT(TestEvent0), &state0);
    TEST_ASSERT_EQUAL(2U, observer[idx].subscriptions);
    TEST_ASSERT_EQUAL(&state, observer[idx].subscriber[0]);
    TEST_ASSERT_EQUAL(&state0, observer[idx].subscriber[1]);
    TEST_ASSERT_EQUAL(NULL, observer[idx].subscriber[2]);
    TEST_ASSERT_EQUAL(NULL, observer[idx].subscriber[3]);
}

extern void EVENTOBSERVERTestSuite(void)
{
    RUN_TEST(test_EVENTOBS_Init);
    RUN_TEST(test_EVENTOBS_Subscribe);
}

