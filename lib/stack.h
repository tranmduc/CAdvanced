#ifndef _STACK_H_
#define _STACK_H_

#include "dllist.h"
#include "jval.h"
/** Implement a stack of Jval using dllist */
typedef Dllist Stack;

/** Create an empty stack*/
Stack createStack();

/** Return is the stack is empty or not */
int isStackEmpty(Stack stack);

/** Add a jval to stack - LIFO*/
void pushStack(Stack stack, Jval val);

/** Retrieve & delete element in stack - LIFO*/
Jval popStack(Stack stack);

/** Free the queue and its elements */
void freeStack(Stack stack);

#endif