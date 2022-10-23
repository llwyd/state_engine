#include "unity.h"
#include "fsm.h"

void setUp( void )
{

}

void tearDown( void )
{

}


void test_TestingUnity( void )
{
    TEST_ASSERT_FALSE( false );
}
   

int main( void )
{
    UNITY_BEGIN();

    RUN_TEST( test_TestingUnity );

    return UNITY_END();
}
