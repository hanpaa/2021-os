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

//우선 전체 탐색 후 나오는 순서 기록.
//먼저 탐색해서 stack에 맨 마지막에 push된것을 뺀다? => stack push 연산 오래걸림.
// => 맨 마지막꺼가 physical memory에 있을법은 없음. 한번 더 탐색해서 나오는 순서 비교해야함.
//table 참조 => 비교하는건 마찬가지지만 그래도 table이니까 속도는 더빠를수도.
//1. 현재 validateBit가 1인 address 탐색 순서 기록.
//2. 순서가 가장 늦은것을 뺀다.
//3. 새로운것을 삽입.
//4. validateBit도 table로 기록.

typedef int element;

typedef struct predictNode{
    
    element value;
    //예시에는 중복 1000개 나올일은 없으니..
    //자바라면 array썻을듯
    int table[500];
    int lastIndex;
    
}predictNode;

typedef struct predictTable{
    
    predictNode* predictTable;
    
}predictTable;

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


stackNode* findLatestNode(stackNode** top, int operationCount, predictTable* table){
    
    stackNode* stack = *top;
    
    stackNode* largestNode = NULL;
    int largestNum =0;
    
    while(stack){
        
        
        for(int i =0; i < table->predictTable[stack->data].lastIndex; i++){
            
            
            //만약 다음 순서가 operation num보다 높으면 stop
            int lastIndex = table->predictTable[stack->data].lastIndex -1;
            
            if(largestNode == NULL || table->predictTable[stack->data].table[lastIndex] < operationCount){
                largestNode = stack;
            }
            //더 없으면 그냥 그것들 Pop해버리면 되는거 아닌가?
            if(table->predictTable[stack->data].table[i] >= operationCount){
                int nextAccessDist = table->predictTable[stack->data].table[i] - operationCount;
                
                if(nextAccessDist > largestNum){
                    largestNum = nextAccessDist;
                    largestNode = stack;
                    
                }
                

                break;
            }
        }
        
        stack = stack -> rLink;
    }
    
    
    
    return largestNode;
    
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

stackNode* findNeverUsed(stackNode** top, int operationCount, predictTable* table){
    
    stackNode* stack = *top;
    
    stackNode* neverUsedNode = NULL;
    int largestNum =0;
    
    while(stack){
        
        
       
        largestNum = table->predictTable[stack->data].lastIndex;
        if(table->predictTable[stack->data].table[largestNum-1] < operationCount){
            neverUsedNode = stack;
            break;
        }
        stack = stack -> rLink;
            }
        
    return neverUsedNode;
    
}




int main(int argc, const char * argv[]) {
    
    
    long inputSize = 0;
    int virtualPageNumber = -1;
    char operation[6];
    int operationCount = 0;
    
    long validBitCount = 0;
    long getFP = 0;
    long getFP2 = 0;
    long isPredict = 0;
    
    long totalAccess = 0;
    long totalRead = 0;
    long totalWrite = 0;
    long totalHit = 0;
    long totalFaults = 0;
    double rate = 0;

    
    srand(time(NULL));
    
    
    
    stackNode* predictStack = createStack();

    scanf("%ld", &inputSize);
    
        
    FILE* fp = fopen("/Users/s85737/Downloads/newfile.list", "r");
    
    if(fp == NULL){
        fprintf(stderr, "unknown Error!");
        exit(-1);
    }
    
    //테이블들 모두 초기화
    predictTable* table = malloc(sizeof(predictTable));
    table->predictTable = (predictNode*)malloc(sizeof(predictNode)*10000);
    
    
    for(int i = 0; i < 10000; i++){
        table->predictTable[i].lastIndex = 0;
        table->predictTable[i].value = -1;
        for (int j = 0; j<500; j++) {
            table->predictTable[i].table[j] = -1 ;
        }
    }
    
    
    //operation 탐색.

    while (!feof(fp)) {
        
        
        //파일에서 operation 우선 탐색.
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
        
        //전체 동작횟수.[fscanf]한 횟수.
        operationCount++;
        
        //현재 파일포인터 저장.
        getFP = ftell(fp);
        
        
        int searchCount = operationCount;
        
        if(isPredict == 0){
            //이미 해당 predictTable완성했으면 안해도됨.
        while (!feof(fp)) {
            
            //predcitTable에서 virtualPageNumber 의 index 접근.
            
            if(table->predictTable[virtualPageNumber].value == -1){
                
                table->predictTable[virtualPageNumber].value = virtualPageNumber;
                //만약 처음 본 값일경우
                table->predictTable[virtualPageNumber].table[0] = searchCount;
                
                table->predictTable[virtualPageNumber].lastIndex++;
                
            }else{
                //만약 한번 봤던거일경우.
                
                long tableLastIndex = table->predictTable[virtualPageNumber].lastIndex;
                table->predictTable[virtualPageNumber].table[tableLastIndex] = searchCount;
                
                table->predictTable[virtualPageNumber].lastIndex++;
                
            }
        
            searchCount++;
            //그다음값 search...
            fscanf(fp, "%d ", &virtualPageNumber);
            fscanf(fp, "%s ", operation);
            
            //마지막값 누락 fscanf 특성상...
            
            ///
            if(feof(fp)){
                
                if(table->predictTable[virtualPageNumber].value == -1){
                    
                    table->predictTable[virtualPageNumber].value = virtualPageNumber;
                    //만약 처음 본 값일경우
                    table->predictTable[virtualPageNumber].table[0] = searchCount;
                    
                    table->predictTable[virtualPageNumber].lastIndex++;
                    
                }else{
                    //만약 한번 봤던거일경우.
                    
                    int tableLastIndex = table->predictTable[virtualPageNumber].lastIndex;
                    table->predictTable[virtualPageNumber].table[tableLastIndex] = searchCount;
                    
                    table->predictTable[virtualPageNumber].lastIndex++;
                }
                
                fclose(fp);
                
                //segement fault 방지 다시 열어줌
                fp = fopen("/Users/s85737/Downloads/newfile.list", "r");
            
                
                //이전 파일포인터로이동
                fseek(fp, getFP, SEEK_SET);
                isPredict = 1;
                break;
            }
            ///
            
        }
        }
        
        //되돌리자. 해당 값의 PredictTable완성
        
        
        
        
        stackNode* targetNode = findLatestNode(&predictStack, operationCount, table);
        
        if(stackSearch(&predictStack, virtualPageNumber) == NULL
           //초반에 모두 falut
           && validBitCount < inputSize){
            
            stackPush(&predictStack, virtualPageNumber);
            totalFaults++;
            validBitCount++;
            
            //이제 안쓰이는 노드 지속적으로 정리
            
            stackNode* neverused = findNeverUsed(&predictStack, operationCount, table);
            if(neverused){
                deleteNode(&predictStack,neverused);
                validBitCount--;
            }

        }else if(stackSearch(&predictStack, virtualPageNumber) != NULL){
            totalHit++;
            
            
            //이제 안쓰이는 노드 지속적으로 정리
            stackNode* neverused = findNeverUsed(&predictStack, operationCount, table);
            if(neverused){
                deleteNode(&predictStack,neverused);
                validBitCount--;
            }
            
            
        }else if(stackSearch(&predictStack, virtualPageNumber) == NULL
                 && validBitCount == inputSize
                 //초반에 모두 falut
                ){
            //physical memory 다찼을경우, fault가 났을때, 가장 멀리있는거 교체
            
        
            deleteNode(&predictStack, findLatestNode(&predictStack, operationCount, table));
            stackPush(&predictStack, virtualPageNumber);
            
            //이제 안쓰이는 노드 지속적으로 정리
            
            stackNode* neverused = findNeverUsed(&predictStack, operationCount, table);
            if(neverused){
                deleteNode(&predictStack,neverused);
                validBitCount--;
            }
            

            totalFaults++;
            
        }
    
    }
        
    
       
        
        
        
    
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
    


