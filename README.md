# C Boilerplate

[![Unit Test CI](https://github.com/llwyd/boilerplate/actions/workflows/unit-tests.yml/badge.svg)](https://github.com/llwyd/boilerplate/actions/workflows/unit-tests.yml)

Personalised boilerplate code so that I don't have to rewrite it for every project.

## Summary

- `emitter_base.c`
    - base class for an event emitter which can be used to enqueue events and configure repeated events via a user-defined timer.
- `fifo_base.c`
    -  FIFO 'base class' with functionality for enqueuing, dequeuing, peeking etc for any particular type.
- `heap_base.c`
    -  Support for min-heaps
- `state.c`
    - This is my personalised take on the UML state machine design pattern popularised by Miro Samek's writings about state machines (which are fantastic).

# Further reading / references / inspiration
* [1] [Introduction to Hierarchical State Machines](https://barrgroup.com/embedded-systems/how-to/introduction-hierarchical-state-machines)
* [2] [UML State Machine (Wiki)](https://en.wikipedia.org/wiki/UML_state_machine)
* [3] [Modern Embedded Programming Course](https://github.com/QuantumLeaps/modern-embedded-programming-course)
* [4] [Object-Oriented Programming in C](https://www.state-machine.com/doc/AN_Simple_OOP_in_C.pdf)
* [5] [Algorithms 4th Edition](https://algs4.cs.princeton.edu/home/)
* [6] [Heap (Data Structure)](https://en.wikipedia.org/wiki/Heap_(data_structure))
