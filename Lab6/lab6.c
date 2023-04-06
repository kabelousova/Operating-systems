#include <stdio.h>
#include <pthread.h>
#include <stdlib.h>
#include <stdbool.h>
#include <unistd.h>
#include <string.h>

#define COEF 20000
#define MAXNUMSTR 100

void* printString(void *args){
    usleep (COEF * strlen((char*)args));
    printf("%s", (char*)args);
}

void freeStrings(char **strings, int strCount){ //освобождаем память
    for (int i = 0; i < strCount; i++){
        free(strings[i]);
    }
    free(strings);
}

int main() {

    //char** strings = malloc(sizeof(char *) * MAXNUMSTR); //массив указателей на char о каждому указателю запишу строку (массив)
     char* strings[MAXNUMSTR];

    int readCount = 2;
    int idx = 0;
    size_t length = 200;

    printf("Enter strings to sort:\n");

    while (readCount > 1 && idx < MAXNUMSTR){
        strings[idx] = malloc(sizeof(char) * length);
        fgets(strings[idx], length, stdin);
        readCount = strlen(strings[idx]);
        idx++;

        if (readCount == 1){
            free(strings[idx]);
            idx--;
        }
    }

    int strCount = idx;

    printf("Sorting your strings. . .\n");

    pthread_t* threads = malloc(sizeof(pthread_t) * strCount);

    for (int i = 0; i < strCount; i++){ //создаем нити
        if (pthread_create(&threads[i], NULL, printString, strings[i]) != 0){
            perror("failed to create new thread");
            freeStrings(strings, strCount);
            free(threads);
            exit(-1);
        }
    }

    for (int i = 0; i < strCount; i++){ // ожидания завершения потоков (для синхронизации)
        if (pthread_join(threads[i], NULL) != 0){
            perror("failed to join thread");
            freeStrings(strings, strCount);
            free(threads);
            exit(-1);
        }
        free(strings[i]);
    }

    free(strings);
    free(threads);
    return 0;
}
