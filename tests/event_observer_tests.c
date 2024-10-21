#include "state.h"
#include "event_observer.h"
#include "unity.h"
#include <string.h>

#define SIGNALS(SIG) \
    SIG(TestEvent0) \
    SIG(TestEvent1) \
    SIG(TestEvent2) \

GENERATE_SIGNALS( SIGNALS );
GENERATE_EVENT_OBSERVERS( SIGNALS );

void test_EVENTOBS_Init(void)
{
    for(uint32_t idx = 0; idx < EVENT(EventCount); idx++)
    {
        TEST_ASSERT_EQUAL( 0U, observer[idx].subscriptions);
    }
}

extern void EVENTOBSERVERTestSuite(void)
{
    RUN_TEST(test_EVENTOBS_Init);
}
