tests: tests.o fsm.o unity.o
	gcc -std=gnu11 tests.o fsm.o unity.o -o tests
tests.o: tests.c
	gcc -std=gnu11 -Wall -g -I../src -I../unity -DUNIT_TESTS -c tests.c -o tests.o
fsm.o: ../src/fsm.c
	gcc -std=gnu11 -Wall -Wpedantic -Wpointer-arith -g -c -DUNIT_TESTS ../src/fsm.c -o fsm.o
unity.o: ../unity/unity.c
	gcc -std=gnu11 -c -I/unity ../unity/unity.c -o unity.o
clean:
	rm fsm.o tests.o unity.o tests
