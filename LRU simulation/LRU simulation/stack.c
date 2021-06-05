//
//  stack.c
//  LRU simulation
//
//  Created by 최제현 on 2021/06/05.
//

#include "stack.h"
#include <stdio.h>
#include <stdlib.h>


stackNode** createStack(){
    return NULL;
}

int stackEmptycheck(stackNode* top){
    if(top == NULL) return 1;
    else return 0;
}


element stackPop(stackNode** top){
    
    element data;
    stackNode** temp;
    
    if(stackEmptycheck(*top))
    {
        fprintf(stderr, "stack empty error!\n");
    }
    
    temp = *top;
    data = (*top) -> data;
    *top = (*top) -> link;
    free(temp);
    
    return data;
}

void stackPush(stackNode** top, element data){
    
    stackNode* newNode = (stackNode*)malloc(sizeof(stackNode));
    
    newNode -> data = data;
    newNode -> link = *top;
    *top = newNode;
    
}

void printStack(stackNode** top){
    
    stackNode* stack = *top;
    
    printf("\n stack [top- ");
    while(stack){
        printf("%d ", stack->data);
        stack = stack -> link;
    }
    printf("]\n");
}
