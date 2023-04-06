#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

#define EXECUTE 1

void FirstFunc(void *args){
    printf("goodbye #%lu\n", pthread_self());
}

void* printString(void *args){
    pthread_cleanup_push(FirstFunc, NULL);
    while (true){
        printf("hello\n");
        pthread_testcancel();
    }
    pthread_cleanup_pop(EXECUTE);
    pthread_exit(0);
}

int main() {

    pthread_t Thread;

    if (pthread_create(&Thread, NULL, printString, NULL) != 0){
        perror("failed to create new thread");
        exit(-1);
    }

    sleep(2);
    printf("\n");

    if (pthread_cancel(Thread) != 0){
        perror("failed to cancel execution of new thread");
        exit(-1);
    }

    if (pthread_join(Thread, NULL) != 0){
        perror("failed to join new thread");
        exit(-1);
    }
    return 0;
}
