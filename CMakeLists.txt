
cmake_minimum_required(VERSION 3.2)

project( stateengine )

set (CMAKE_C_STANDARD 11 )
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY bin/ )
set (CMAKE_BUILD_TYPE Debug )

include_directories( tests.out PRIVATE Unity )
include_directories( tests.out PRIVATE Unity/src )
include_directories( tests.out PRIVATE src )

add_executable( tests.out
                src/assert_bp.h
                src/heap_base.h
                src/heap_base.c
                src/fifo_base.h
                src/fifo_base.c
                src/state.c
                src/state.h
                src/state_history.c
                src/state_history.h
                src/emitter_base.h
                src/emitter_base.c
                src/event_observer.h
                tests/fifo_tests.c
                tests/fifo_tests.h
                tests/state_tests.c
                tests/state_tests.h
                tests/heap_tests.h
                tests/heap_tests.c
                tests/tests.c
                tests/emitter_tests.h
                tests/emitter_tests.c
                tests/event_observer_tests.h
                tests/event_observer_tests.c
                Unity/src/unity.c
                Unity/src/unity.h
                Unity/src/unity_internals.h ) 


target_compile_options( tests.out
                        PUBLIC
                        -Wfatal-errors
                        #-Wall
                        #-Wextra
                        -Werror
                        #-Wpedantic
                        #-Wpointer-arith
                        -g
                        -DUNIT_TESTS
                        -DUNITY_OUTPUT_COLOR )
