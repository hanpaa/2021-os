//
//  queue.c
//  LRU simulation
//
//  Created by 최제현 on 2021/06/05.
//

#include "queue.h"
#include <stdlib.h>
#include <stdio.h>


int isQueueEmpty(queueSub* queue){
    if(!(queue->front)) return 1;
    else return 0;
}

queueSub* createQueueSub(queueSub* queue){
    
    queue = (queueSub*)malloc(sizeof(queueSub));
    
    queue -> rear = NULL;
    queue -> front = NULL;
    
    return queue;
    
    
}

void enqueue(queueSub* queue, element data){
    
    queueNode* newNode = (queueNode*)malloc(sizeof(queueNode));
    
    newNode -> data = data;
    
    if((queue -> rear == NULL))
    {
        newNode -> rlink = NULL;
        queue -> front = newNode;
        queue -> rear = newNode;
    }else
    {
        newNode -> rlink = queue -> rear;
        queue -> rear -> llink = newNode;
        queue -> rear = newNode;
    }
    
}

element dequeue(queueSub* queue){
    
    queueNode* temp = queue -> front;
    
    element data;
    
    if(queue -> front == queue -> rear)
    {
        data = queue -> front -> data;
        queue -> front = NULL;
        queue -> rear = NULL;
        free(temp);
    }else
    {
        data = queue -> front -> data;
        queue -> front = queue -> front -> llink;
        queue -> front -> rlink = NULL;
        if(queue -> front == NULL) queue -> rear = NULL;
        free(temp);
    }
    
    return data;
}

void printQueue(queueSub* queue){
    
    queueNode* node = queue -> rear;
    if(isQueueEmpty(queue))
    {
        fprintf(stderr, "queue is empty");
        return;
    }
    printf("\nQueue [ rear ");
    while(node){
        printf("<-[%d]->", node -> data);
        node = node -> rlink;
    }
    printf(" front ]\n");
}
