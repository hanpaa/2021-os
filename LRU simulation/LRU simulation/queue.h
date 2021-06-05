//
//  queue.h
//  LRU simulation
//
//  Created by 최제현 on 2021/06/05.
//

#ifndef queue_h
#define queue_h

#include <stdio.h>

typedef int element;

typedef struct queueSub{
    
    struct queueNode* front;
    struct queueNode* rear;

}queueSub;

typedef struct queueNode{
    
    element data;
    struct queueNode* rlink;
    struct queueNode* llink;
    
}queueNode;

int isQueueEmpty(queueSub* queue);
queueSub* createQueueSub(queueSub* queue);
void enqueue(queueSub* queue, element data);
element dequeue(queueSub* queue);
void printQueue(queueSub* queue);


#endif /* queue_h */
