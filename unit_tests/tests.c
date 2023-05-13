#include "state_tests.h"
#include "fifo_tests.h"
#include "unity.h"

int main( void )
{
    UNITY_BEGIN();

    FIFOTestSuite();
    STATETestSuite();

    return UNITY_END();
}
