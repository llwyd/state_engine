fsm_example: fsm_example.o fsm.o
	gcc -std=gnu11 fsm_example.o fsm.o -o fsm_example
fsm_example.o: fsm_example.c
	gcc -std=gnu11 -Wall -g -I../src -c fsm_example.c -o fsm_example.o
fsm.o: ../src/fsm.c
	gcc -std=gnu11 -Wall -g -c ../src/fsm.c -o fsm.o
clean:
	rm fsm.o fsm_example.o fsm_example
