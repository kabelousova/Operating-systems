#include <stdio.h>
#include <pthread.h>
#include <unistd.h>


pthread_cond_t cv;
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

volatile int who = 0;

struct setup {
    char *text;
    int who_am_i;
};


void *worker(void *ptr) {
    char *text = ((struct setup *) ptr)->text; //приняли пере
    int who_am_i = ((struct setup *) ptr)->who_am_i;

    for (int i = 0; i < 10; i++) {
        // Поток, пришедший сюда вторым, ждет пока код ниже не исполнится *первым*
        // Выполнение кода *вторым* потоком блокируется до тех пор, пока мьютекс не будет освобождён *первым* потоком
        pthread_mutex_lock(&mutex);
        if (who_am_i != who) {
            pthread_cond_wait(&cv, &mutex); // Если сюда попал "ненужный" поток, то мы блокируем его с использованием
            // условной переменной и освобождаем "нужный" поток (+освобождаем у него mutex)
        }
        printf("%s\n", text); // Печатаем текст на "нужном" потоке
        who = (who + 1) % 2;
        pthread_mutex_unlock(&mutex); // Освобождаем mutex у "нужного" потока
        pthread_cond_signal(
                &cv); // Разблокируем "ненужный" поток, заблокированный с использованием условной переменной,
        // "ненужный" поток выходит из if печатает свой текст
    }

    return NULL;
}


int main() {
    pthread_cond_init(&cv, NULL); //инициализируем условную переменную

    struct setup parent = { //заполняем структуры родителю и ребенку
            .text = "Parent",
            .who_am_i = 0
    };

    struct setup child = {
            .text = "Child",
            .who_am_i = 1
    };


    pthread_t thread;
    if (pthread_create(&thread, NULL, worker, &child) != 0) {
        perror("error creating thread");
        return 1;
    }

    worker(&parent);

    pthread_exit(NULL);
}
