#ifndef _QUEUE_H_
#define _QUEUE_H_

#include "dllist.h"
#include "jval.h"
/** Implement a queue of Jval using dllist */
typedef Dllist Queue;

/** Create an empty queue*/
Queue createQueue();

/** Return is the queue is empty or not */
int isQueueEmpty(Queue queue);

/** Add a jval to queue - FIFO*/
void pushQueue(Queue queue, Jval val);

/** Retrieve & delete element in queue - FIFO*/
Jval popQueue(Queue queue);

/** Free the queue and its elements */
void freeQueue(Queue queue);

#endif