#ifndef ASSERT_BP_H
#define ASSERT_BP_H

#ifdef TARGET_ARM
    #define ASSERT( c ) \
        { \
            if ( !(c) ) \
            { \
                asm("CPSID IF"); \
                while(1) \
                { \
                } \
            } \
        } 
#else
    #include <assert.h>

    #define ASSERT( c ) \
    { \
        assert( (c) ); \
    } 
#endif

#endif /* ASSERT_BP_H */
