# Boilerplate

Various boilerplate code I've made for use in projects.

## Summary

- state.c
    - This is my personal take on that UML state machine design pattern that I use in my various hobby projects.  I've also included a circular buffer event queue for storing and emitting events to the state machine.  This is generally inspired by Miro Samek's design pattern and writings about state machines which I think are fantastic.
- fifo_base.c
    - An attempt at a fifo 'base class' with functionality for enqueuing, dequeuing etc for any particular type.

# Further reading / references / inspiration
* [1] [Introduction to Hierarchical State Machines](https://barrgroup.com/embedded-systems/how-to/introduction-hierarchical-state-machines)
* [2] [UML State Machine (Wiki)](https://en.wikipedia.org/wiki/UML_state_machine)
* [3] [Modern Embedded Programming Course](https://github.com/QuantumLeaps/modern-embedded-programming-course)
* [4] [Object-Oriented Programming in C](https://www.state-machine.com/doc/AN_Simple_OOP_in_C.pdf)
