#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

enum errors{
    man = 3,
    woman = 2,
    success = 1,
    wrong_input = -1,
    woman_leave = -2,
    man_leave = -3
};

int max_people;
int women_inside = 0;
int man_inside = 0;
pthread_cond_t woman_entered;
pthread_cond_t man_entered;
pthread_mutex_t mutex;

void simulate_bath(int man_number, int woman_number);
void *woman_thread(void *person);
void *man_thread(void *person);
void woman_wants_to_enter();
void man_wants_to_enter();
void woman_leaves();
void man_leaves();
void print_bathroom_state();
void print(int state);

int main(int argc, char *argv[]){
    if(argc != 2){
        print(wrong_input);
        return wrong_input;
    }
    max_people = atoi(argv[1]);

    pthread_cond_init(&woman_entered, NULL);
    pthread_cond_init(&man_entered, NULL);
    pthread_mutex_init(&mutex, NULL);

    int man_number = 5;
    int woman_number = 5;

    simulate_bath(man_number, woman_number);

    pthread_cond_destroy(&woman_entered);
    pthread_cond_destroy(&man_entered);
    pthread_mutex_destroy(&mutex);
    return success;
}

void simulate_bath(int man_number, int woman_number){
    pthread_t women_th[woman_number];
    pthread_t men_th[man_number];
    int person = 1;

    for(int i = 0, j = 0; i < woman_number, j < man_number; i++, j++){
        pthread_create(&women_th[i], NULL, woman_thread, &person);
        pthread_create(&men_th[j], NULL, man_thread, &person);
    }
    for (int i = 0, j = 0; i < woman_number, j < man_number; i++, j++) {
        pthread_join(women_th[i], NULL);
        pthread_join(men_th[j], NULL);
    }
}

void *woman_thread(void *person){
    int number = *((int *)person);
    for(int i = 0; i < number; i++){
        woman_wants_to_enter();
        woman_leaves();
    }
    return NULL;
}

void *man_thread(void *person){
    int number = *((int *)person);
    for(int i = 0; i < number; i++){
        man_wants_to_enter();
        man_leaves();
    }
    return NULL;
}

void woman_wants_to_enter(){
    pthread_mutex_lock(&mutex);
    while(man_inside > 0 || women_inside == max_people){
        pthread_cond_wait(&woman_entered, &mutex);
    }
    print(woman);
    women_inside++;
    print_bathroom_state();
    pthread_mutex_unlock(&mutex);
}

void man_wants_to_enter(){
    pthread_mutex_lock(&mutex);
    while(women_inside > 0 || man_inside == max_people){
        pthread_cond_wait(&man_entered, &mutex);
    }
    print(man);
    man_inside++;
    print_bathroom_state();
    pthread_mutex_unlock(&mutex);
}

void woman_leaves(){
    pthread_mutex_lock(&mutex);
    women_inside--;
    if(women_inside == 0){
        pthread_cond_broadcast(&man_entered);
    }
    print(woman_leave);
    print_bathroom_state();
    pthread_mutex_unlock(&mutex);
}

void man_leaves(){
    pthread_mutex_lock(&mutex);
    man_inside--;
    if(man_inside == 0){
        pthread_cond_broadcast(&woman_entered);
    }
    print(man_leave);
    print_bathroom_state();
    pthread_mutex_unlock(&mutex);
}

void print(int state){
    if(state == wrong_input){
        printf("Input is wrong");
    }
    else if(state == woman){
        printf("Woman is in bathroom\n");
    }
    else if(state == man){
        printf("Man is in bathroom\n");
    }
    else if(state == woman_leave){
        printf("Woman leaves bathroom\n");
    }
    else if(state == man_leave){
        printf("Man leaves bathroom\n");
    }
}

void print_bathroom_state(){
    printf("Bathroom state: Women inside = %d, Men inside = %d\n\n", women_inside, man_inside);
}