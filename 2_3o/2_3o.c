#include <stdio.h>
#include <pthread.h>
#include <semaphore.h>
#include <unistd.h>
#include <stdlib.h>

sem_t forks[5];
sem_t mutex;
void *phil_process(void *arg);

int main() {
    pthread_t phils[5];
    int phils_id[5] = {0, 1, 2, 3, 4};

    sem_init(&mutex, 0, 1);

    for(int i = 0; i < 5; i++){
        sem_init(&forks[i], 0, 1);
    }    

    for(int i = 0; i < 5; i++){
        pthread_create(&phils[i], NULL, phil_process, &phils_id[i]);
    }

    for(int i = 0; i < 5; i++){
        pthread_join(phils[i], NULL);
    }

    sem_destroy(&mutex);

    for(int i = 0; i < 5; i++){
        sem_destroy(&forks[i]);
    }
    return 0;
}

void *phil_process(void *arg){
    int id = *(int *)arg;
    while (1) {
        printf("Philosopher %d is thinking\n", id);
        sleep(1);
        printf("Philosopher %d is hungry\n", id);

        //sleep(rand() % 3);
        sem_wait(&mutex); //блокировка доступа к вилкам

        sem_wait(&forks[id]); //захват виоки
        printf("Philosopher %d take fork %d\n", id, id);

        sem_wait(&forks[(id + 1) % 5]);
        printf("Philosopher %d take fork %d\n", id, (id + 1) % 5);

        printf("Philosopher %d is eating\n", id);

        sem_post(&forks[id]); //освобождение
        sem_post(&forks[(id + 1) % 5]);

        sem_post(&mutex); //открытие доступа

        printf("Philosopher %d is free\n", id);
        sleep(1);
    }
}