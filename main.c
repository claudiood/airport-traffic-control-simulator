#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include "resources.h"

#define NUM_FLIGHTS 15

int main(){

    printf("Starting simulation!\n");
    
    init_resources();
    
    pthread_t flight_threads[NUM_FLIGHTS];

    Flight *flights[NUM_FLIGHTS];

    for(int i = 0; i < NUM_FLIGHTS; i++){
        flights[i] = (Flight *)malloc(sizeof(Flight));
        if(flights[i] == NULL){
            printf("Error!");
            exit(EXIT_FAILURE);
        }

        flights[i]->id = i + 1;
        flights[i]->type = (i%2 == 0 ? INTERNATIONAL : NATIONAL);
        flights[i]->state = READY;

        if(pthread_create(&flight_threads[i], NULL, simulate_flight, (void *)flights[i]) != 0){
            printf("Error");
            exit(EXIT_FAILURE);
        }
    }

    for(int i = 0; i < NUM_FLIGHTS; i++){
        pthread_join(flight_threads[i], NULL);
    }

    destroy_resources();

    printf("Finishing simulation!\n");

    exit(EXIT_SUCCESS);
}