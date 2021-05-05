//
//  main.c
//  os_HW1
//
//  Created by 최제현 on 2021/05/05.
//

#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>

void *getSum(int *arg);
void *getFactorial(int *arg);

int main(int argc, const char * argv[]) {
    int num;
    int result[2];
    int i, returnCode, returnCode2;
    pthread_t threads[2];
    
    
    // 입력받음
    scanf("%d", &num);

    
//    https://man7.org/linux/man-pages/man3/pthread_create.3.html
//    thread 생성
    returnCode = pthread_create(&threads[0], NULL, (void*)&getSum, (void*)&num);
    returnCode2 = pthread_create(&threads[1], NULL, (void*)&getFactorial, (void*)&num);
    
//    error 발생시 출력
    if(returnCode != 0 || returnCode2 != 0){
        fprintf(stderr, "error : returnCode = %d\n", returnCode);
        fprintf(stderr, "error : returnCode2 = %d\n", returnCode2);
        exit(1);
    }
    
//    main thread sleep
    
    for(i = 0; i < 2; i++)
    {
        
//         https://man7.org/linux/man-pages/man3/pthread_join.3.html
//        thread 가 terminate 될때까지 기다린후 Join, 자원 반환
        returnCode = pthread_join(threads[i], (void*)&result[i]);
        
//        error 발생시 출력
        if(returnCode != 0)
        {
            fprintf(stderr,"Error thread %d | error code : %d\n", i, returnCode);
            exit(1);
        }
    }
    
    //결과 출력
    
    printf("sum = %d\n", result[0]);
    printf("factorial = %d\n", result[1]);
    

    
    return 0;
}


//입력 타입 변경을 위해서는 arg를 void로 둔다.

/**
 0~N까지 sum값 출력하는 함수
 */
void* getSum(int *arg)
{
    long result = 0;
    int num = *arg;
    
    for(int i = 1; i <= num; i++){
        result += i;
    }

//    https://man7.org/linux/man-pages/man3/pthread_exit.3.html
//    thread 종료 result return -> result는 void형 포인터로
    pthread_exit((void*)result);
    
}

/**
 0~N까지 factorial값 출력하는 함수
 */
void* getFactorial(int *arg)
{
    long result = 1;
    int num = *arg;
    
    for (int i = 1; i <= num; i++) {
        result *= i;
    }
    
    
    //    thread 종료 result return -> result는 void형 포인터로
        pthread_exit((void*)result);
    
}
