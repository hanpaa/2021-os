//
//  stack.h
//  LRU simulation
//
//  Created by 최제현 on 2021/06/05.
//

#ifndef stack_h
#define stack_h

#include <stdio.h>

typedef int element;

typedef struct stackNode{
    
    element data;
    struct stackNode* link;
    
    
}stackNode;

typedef int element;
stackNode** createStack();
int stackEmptycheck(stackNode* top);
element stackPop(stackNode** top);
void stackPush(stackNode** top, element data);
void printStack(stackNode** top);

#endif /* stack_h */
