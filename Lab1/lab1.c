#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void* printStrings(void *args) { //thread function
    for (int i = 0; i < 10; i++) {
        printf("Child thread: String #%d\n", i);
    }
    return((void *)1);
}

int main() {
    pthread_t thread; //thread id
    int status = pthread_create(&thread, NULL, printStrings, NULL);

    if (status != 0) {
        printf("failed to create thread. Status is %d\n", status);
        exit(-1);
    }

    for (int i = 0; i < 10; i++) {
        printf("Parent thread: String #%d\n", i);
    }

    return 0;
}
