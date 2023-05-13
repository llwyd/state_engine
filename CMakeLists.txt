
cmake_minimum_required(VERSION 3.2)

project( stateengine )

set (CMAKE_C_STANDARD 11 )
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY bin/ )
set (CMAKE_BUILD_TYPE Debug )

include_directories( tests.out PRIVATE unity )
include_directories( tests.out PRIVATE src )

add_executable( tests.out
                src/state.c
                src/state.h
                unit_tests/tests.c
                unity/unity.c
                unity/unity.h
                unity/unity_internals.h ) 


target_compile_options( tests.out
                        PUBLIC
                        -Wall
                        -Werror
                        #-Wpedantic
                        -Wpointer-arith
                        -g
                        -DUNIT_TESTS )
