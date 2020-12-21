#include "queue.h"
#include <stdio.h>

/** Create an empty queue*/
Queue createQueue(){
    return new_dllist();
}

/** Return is the queue is empty or not */
int isQueueEmpty(Queue queue){
    return dll_empty(queue);
}

/** Add a jval to queue - FIFO*/
void pushQueue(Queue queue, Jval val){
    return dll_prepend(queue, val);
}

/** Retrieve & delete element in queue - FIFO*/
Jval popQueue(Queue queue){
    //check emptiness
    if(isQueueEmpty(queue)){
        printf("Err: Queue is empty, cannot pop element\n");
        return;
    }

    Dllist node = dll_last(queue);
    Jval return_jval = dll_val(node);
    
    dll_delete_node(node);

    return return_jval;
}

/** Free the queue and its elements */
void freeQueue(Queue queue){
    return free_dllist(queue);
}