#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include "resources.h"

int main(int argc, char *argv[]){

    validate_resources(argc, argv);
    
    printf("Starting simulation of %d minutes!\n", provided_time);
    printf("Resources: %d runways, %d gates and %d towers slots.\n", available_runways, available_gates, available_tower_slots);

    srand(time(NULL));

    time_t start_time = time(NULL);
    time_t current_time;

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

    free_threads(flight_threads, flights);
    destroy_resources();

    printf("Finishing simulation!\n");

    printf("------------------------SUMMARY--------------------------\n");
    printf("INTERNATIONAL FLIGHTS SUCESSESS: %d\n", international_sucessess);
    printf("NATIONAL FLIGHTS SUCESSESS: %d\n", national_sucessess);
    printf("NATIONAL FLIGHTS CRASHES: %d\n", national_crashes);
    printf("---------------------------------------------------------\n");
    exit(EXIT_SUCCESS);
}