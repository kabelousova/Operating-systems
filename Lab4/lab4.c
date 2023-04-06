#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>

void* printString(void *args){
    while (true){
        printf("hello ");
    }
}

int main() {

    pthread_t thread;
    int status = pthread_create(&thread, NULL, printString, NULL);

    if (status != 0){
        perror("failed to create new thread");
        exit(-1);
    }

    sleep(2);

    status = pthread_cancel(thread);

    printf("\n");

    if (status != 0){
        perror("failed to cancel execution of new thread");
        exit(-1);
    }

    return 0;
}
