#include "state_tests.h"
#include "fifo_tests.h"
#include "heap_tests.h"
#include "multi_state_tests.h"
#include "unity.h"

int main( void )
{
    UNITY_BEGIN();

    FIFOTestSuite();
    STATETestSuite();
    HeapTestSuite();
    MULTISTATETestSuite();
    return UNITY_END();
}
