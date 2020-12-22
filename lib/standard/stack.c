#include "stack.h"
#include <stdio.h>

/** Create an empty stack*/
Stack createStack(){
    return new_dllist();
}

/** Return is the stack is empty or not */
int isStackEmpty(Stack stack){
    return dll_empty(stack);
}

/** Add a jval to stack - LIFO*/
void pushStack(Stack stack, Jval val){
    return dll_prepend(stack, val);
}

/** Retrieve & delete element in stack - LIFO*/
Jval popStack(Stack stack){
    //check emptiness
    if(isStackEmpty(stack)){
        printf("Err: Stack is empty, cannot pop element\n");
        return;
    }

    Dllist node = dll_first(stack);
    Jval return_jval = dll_val(node);
    
    dll_delete_node(node);

    return return_jval;
}

/** Free the queue and its elements */
void freeStack(Stack stack){
    return free_dllist(stack);
}