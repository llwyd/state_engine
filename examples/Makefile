state_example: state_example.o state.o
	gcc -std=gnu11 state_example.o state.o -o state_example
state_example.o: state_example.c
	gcc -std=gnu11 -Wall -g -I../src -c state_example.c -o state_example.o
state.o: ../src/state.c
	gcc -std=gnu11 -Wall -Wpedantic -Wpointer-arith -g -c ../src/state.c -o state.o
clean:
	rm state.o state_example.o state_example
