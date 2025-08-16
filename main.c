#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "resources.h"

#define True 1
#define DEFAULT_SIMULATION_TIME 120

int main(int argc, char *argv[]){

    short int simulation_time = DEFAULT_SIMULATION_TIME;
    short int provided_time = DEFAULT_SIMULATION_TIME/60;

    if(argc > 1){
        provided_time = atoi(argv[1]);
        simulation_time = provided_time * 60;
        if(provided_time <= 0){
            provided_time = DEFAULT_SIMULATION_TIME/60;
            simulation_time = DEFAULT_SIMULATION_TIME;
            printf("Invalid simulation time provided. Default simulation time of %d minutes running.\n", provided_time);
        }
    }
    
    printf("Starting simulation of %d minutes!\n", provided_time);

    srand(time(NULL));

    time_t start_time = time(NULL);
    time_t current_time;

    short int num_threads = 0;

    pthread_t *flight_threads = NULL;
    Flight **flights = NULL;

    init_resources();

    while(True){
        current_time = time(NULL);

        if(difftime(current_time, start_time) > simulation_time){
            break;
        }

        flights = (Flight **)realloc(flights, (num_threads + 1)*sizeof(Flight*));
        flight_threads = (pthread_t*)realloc(flight_threads, (num_threads + 1) * sizeof(pthread_t));

        flights[num_threads] = (Flight *)malloc(sizeof(Flight));

        flights[num_threads]->id = num_threads + 1;
        flights[num_threads]->type = (rand() % 2 == 0 ? INTERNATIONAL : NATIONAL);
        flights[num_threads]->created_At = time(NULL);
        flights[num_threads]->status = NORMAL;

        pthread_create(&flight_threads[num_threads], NULL, simulate_flight, (void *)flights[num_threads]);

        num_threads++;

        sleep(1);
    }

    printf("End of simulation! Waiting for the last flights!\n");

    for(int i = 0; i < num_threads; i++){
        pthread_join(flight_threads[i], NULL);
        free(flights[i]);
    }

    free(flights);
    free(flight_threads);

    destroy_resources();

    printf("Finishing simulation!\n");

    exit(EXIT_SUCCESS);
}