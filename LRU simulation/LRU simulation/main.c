//
//  main.c
//  LRU simulation
//
//  Created by 최제현 on 2021/06/05.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

typedef int element;

typedef struct stackNode{
    
    element data;
    struct stackNode* rLink;
    struct stackNode* lLink;
    
}stackNode;

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
    *top = (*top) -> rLink;
    
    free(temp);
    
    return data;
}

void stackPush(stackNode** top, element data){
    
    stackNode* newNode = (stackNode*)malloc(sizeof(stackNode));
    
    newNode -> data = data;
    newNode -> rLink = *top;
    if(*top != NULL)
    (*top)->lLink = newNode;
    *top = newNode;
    newNode -> lLink = NULL;
    
}

void printStack(stackNode** top){
    
    stackNode* stack = *top;
    
    printf("\n stack [top- ");
    while(stack){
        printf("%d ", stack->data);
        stack = stack -> rLink;
    }
    printf("]\n");
}

stackNode* stackSearch(stackNode** top, int data){
    
    stackNode* stack = *top;
    while(stack){
        if(stack->data == data) return stack;
        else{stack = stack -> rLink;}
    }
    
    return NULL;
}

void deleteNode(stackNode** top, stackNode* deleteNode)
{
    
    if (*top == NULL || deleteNode == NULL)
        return;
 
    
    if (*top == deleteNode)
        *top = deleteNode->rLink;
 
    
    if (deleteNode->rLink != NULL)
        deleteNode->rLink->lLink = deleteNode->lLink;
    else
        if(deleteNode -> lLink != NULL) deleteNode ->lLink->rLink = NULL;
 
    
    if (deleteNode->lLink != NULL)
        deleteNode->lLink->rLink = deleteNode->rLink;
    else
        if(deleteNode -> rLink != NULL) deleteNode->rLink->lLink = NULL;
 

    free(deleteNode);
    
    return;
}


stackNode* buttomSearch(stackNode** top){
    
    stackNode* stack = *top;
    while(stack){
        if(stack->rLink == NULL) return stack;
        else{stack = stack -> rLink;}
    }
    
    return NULL;
}



int main(int argc, const char * argv[]) {
    
    
    long inputSize = 0;
    int virtualPageNumber = -1;
    char operation[6];
    int operationCount = 0;
    long validBitCount = 0;
    
    long totalAccess = 0;
    long totalRead = 0;
    long totalWrite = 0;
    long totalHit = 0;
    long totalFaults = 0;
    double rate = 0;

    
    srand(time(NULL));
    
    
    stackNode* top = createStack();

    scanf("%ld", &inputSize);
    
        
    FILE* fp = fopen("access.list", "r");
    
    if(fp == NULL){
        fprintf(stderr, "unknown Error!");
        exit(-1);
    }
    
    

    while (!feof(fp)) {
        
        
        fscanf(fp, "%d ", &virtualPageNumber);
        fscanf(fp, "%s ", operation);
        
        if(strcmp(operation, "read") == 0){
            totalRead++;
        }else if(strcmp(operation, "write") == 0) {
            totalWrite++;
        }else{
            fscanf(stderr, "fileError");
            exit(-1);
        }
        
        
        if(stackSearch(&top, virtualPageNumber) == NULL){
            
            stackPush(&top, virtualPageNumber);
            totalFaults++;
            validBitCount++;

        }else if(stackSearch(&top, virtualPageNumber) != NULL){
            
            deleteNode(&top, stackSearch(&top, virtualPageNumber));
            stackPush(&top, virtualPageNumber);
            totalHit++;
            
        }else if(stackSearch(&top, virtualPageNumber) == NULL
        &&validBitCount == inputSize){
            
            totalFaults++;
            deleteNode(&top, buttomSearch(&top));
            stackPush(&top, virtualPageNumber);
            
        }
        
            
        }
        
       
        operationCount++;
        
        
    
    totalAccess = totalRead + totalWrite;
    
    rate = (double)totalFaults / (double)totalAccess * 100;
    
    
    printf("Total number of access: %ld\n", totalAccess);
    printf("Total number of read: %ld\n", totalRead);
    printf("Total number of write: %ld\n", totalWrite);
    printf("Number of page hits: %ld\n", totalHit);
    printf("Number of page faults: %ld\n", totalFaults);
    printf("Page fault rate: %ld/%ld = %0.3f %%\n",totalFaults,totalAccess, rate);
    
    
    fclose(fp);

    

    return 0;
}


