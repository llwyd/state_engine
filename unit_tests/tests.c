#include "state_tests.h"
#include "fifo_tests.h"


int main( void )
{
    FIFOTestSuite();
    STATETestSuite();

    return 0U;
}
