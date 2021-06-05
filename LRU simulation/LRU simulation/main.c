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
#include "stack.h"


typedef struct pageTable{
    
    int count;
    int validBit;
    long address;
    int referenceBit;
    
}pageTable;

typedef struct tableSruct{
    
    struct pageTable* pageTable;
    
}tableStruct;

int main(int argc, const char * argv[]) {
    
    int diskStorage[60000];
    int inputSize = 0;
    int* physicalMemory = 0;
    int virtualPageNumber = -1;
    int tableSize = 1001;
    char operation[6];
    int operationCount = 0;
    long maxCount = 0;
    int maxCountAddress = -1;
    long validBitCount = 0;
    
    long checkcheck =0;
    
    long totalAccess = 0;
    long totalRead = 0;
    long totalWrite = 0;
    long totalHit = 0;
    long totalFaults = 0;
    float rate = 0;

    
    srand(time(NULL));
    
  
    
    
    scanf("%d", &inputSize);
    
    physicalMemory = (int*)malloc(sizeof(int)*inputSize);
    

    tableStruct* table = (tableStruct*)malloc(sizeof(table));
    table -> pageTable = (pageTable*)malloc(sizeof(pageTable)*tableSize);
    
    
    //table 초기화
    for(int i =0; i < tableSize; i++){
        table->pageTable[i].address = -1;
        table->pageTable[i].validBit = 0;
    }
    
    
    
    
    FILE* fp = fopen("/Users/s85737/Downloads/access.list", "r");
    
    if(fp == NULL){
        fprintf(stderr, "unknown Error!");
        exit(-1);
    }
    
    

    while (!feof(fp)) {
        if((operationCount % 500) == 0){
            operationCount = 0;
            for (int i = 0; i < tableSize; i++) {
                table->pageTable[i].referenceBit = 0;
            }
        }
        
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
        
        
        if(table->pageTable[virtualPageNumber].address == -1 &&
           validBitCount < inputSize){
            
            //아직 physical memory에 할당 안되었을때.
            
            int randAddress = rand() % inputSize;
            table->pageTable[virtualPageNumber].address = randAddress;
            table->pageTable[virtualPageNumber].validBit = 1;
            physicalMemory[randAddress] = 1;
            
            totalFaults++;
            validBitCount++;
            
        }else{
            
            //다 할당된후, 평소 동작
            
            if(table->pageTable[virtualPageNumber].validBit == 1){
                totalHit++;
                //R = 1 이므로, count 0으로
                table->pageTable[virtualPageNumber].referenceBit = 1;
                table->pageTable[virtualPageNumber].count = 0;
                
                for(int searchIndex = 0; searchIndex < tableSize; searchIndex++){
                    // counter
                    if(table->pageTable[searchIndex].referenceBit == 0
                       && table->pageTable[searchIndex].validBit == 1)
                        table->pageTable[searchIndex].count++;
                    
                    if(table->pageTable[searchIndex].count >= maxCount){
                        maxCount = table->pageTable[searchIndex].count;
                        maxCountAddress = searchIndex;
                    }
                }
                
            }else{
                totalFaults++;
                //counter operation
                for(int searchIndex = 0; searchIndex < tableSize; searchIndex++){
                    
                    if(table->pageTable[searchIndex].referenceBit == 0
                       && table->pageTable[searchIndex].validBit == 1)
                        table->pageTable[searchIndex].count++;
                    
                    if(table->pageTable[searchIndex].count >= maxCount
                       && table->pageTable[searchIndex].validBit == 1){
                        maxCount = table->pageTable[searchIndex].count;
                        maxCountAddress = searchIndex;
                    }
                }
                
                if(table->pageTable[maxCountAddress].validBit == 1
                   &&maxCountAddress != -1){
                    
                    table->pageTable[maxCountAddress].validBit = 0;
                    table->pageTable[maxCountAddress].address = rand() % 60000;
                    table->pageTable[maxCountAddress].referenceBit = 0;
                    maxCountAddress = -1;
                    
                }
                
                
            }
            
        }
        
       
        operationCount++;
        
        
        for(int i=0; i<tableSize; i++){
            if(table->pageTable[i].validBit == 1) checkcheck++;
        }
        
        checkcheck = 0;
        
    }
        
    
    totalAccess = totalRead + totalWrite;
    
    rate = totalFaults / totalAccess;
    
    
    printf("Total number of access: %ld\n", totalAccess);
    printf("Total number of read: %ld\n", totalRead);
    printf("Total number of write: %ld\n", totalWrite);
    printf("Number of page hits: %ld\n", totalHit);
    printf("Number of page faults: %ld\n", totalFaults);
    printf("Page fault rate: %lf", rate);
    
    
    fclose(fp);

    

    return 0;
}


