tests: tests.o state.o unity.o
	gcc -std=gnu11 tests.o state.o unity.o -o tests
tests.o: tests.c
	gcc -std=gnu11 -Wall -g -I../src -I../unity -DUNIT_TESTS -c tests.c -o tests.o
state.o: ../src/state.c
	gcc -std=gnu11 -Wall -Wpedantic -Wpointer-arith -g -c -DUNIT_TESTS ../src/state.c -o state.o
unity.o: ../unity/unity.c
	gcc -std=gnu11 -c -I/unity ../unity/unity.c -o unity.o
clean:
	rm state.o tests.o unity.o tests
