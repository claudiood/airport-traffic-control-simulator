#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "flight.h"
#include "resources.h"

short int simulation_time = DEFAULT_SIMULATION_TIME;
short int provided_time = DEFAULT_SIMULATION_TIME/60.0;

short int num_threads = 0;

short int international_flight_waiting_runway = 0;
short int national_flight_waiting_runway = 0;

short int international_flight_waiting_gate = 0;
short int national_flight_waiting_gate = 0;

short int international_flight_waiting_tower = 0;
short int national_flight_waiting_tower = 0;

short int available_runways = DEFAULT_RUNWAYS;
short int available_gates = DEFAULT_GATES;
short int available_tower_slots = DEFAULT_TOWER_SLOTS;

short int international_sucessess = 0;
short int national_sucessess = 0;
short int national_crashes = 0;

pthread_mutex_t summary;

pthread_mutex_t runway_mutex;
pthread_mutex_t tower_mutex;
pthread_mutex_t gate_mutex;

sem_t runway_sem;
sem_t tower_sem;
sem_t gate_sem;

void init_resources(){

    pthread_mutex_init(&runway_mutex, NULL);
    pthread_mutex_init(&tower_mutex, NULL);
    pthread_mutex_init(&gate_mutex, NULL);

    sem_init(&runway_sem, 0, available_runways);
    sem_init(&gate_sem, 0, available_gates);
    sem_init(&tower_sem, 0, available_tower_slots);

    printf("Resources started.\n");
}

void destroy_resources(){
    pthread_mutex_destroy(&runway_mutex);
    pthread_mutex_destroy(&tower_mutex);
    pthread_mutex_destroy(&gate_mutex);

    sem_destroy(&runway_sem);
    sem_destroy(&tower_sem);
    sem_destroy(&gate_sem);
    
    printf("Resources destroyed.\n");
}

void validate_resources(int argc, char *argv[]){

    if(argc == 2){
        provided_time = atoi(argv[1]);
        simulation_time = provided_time * 60;

        if(provided_time <= 0){
            provided_time = DEFAULT_SIMULATION_TIME/60;
            simulation_time = DEFAULT_SIMULATION_TIME;
            printf("Invalid simulation time provided. Default simulation time of %d minutes running.\n", provided_time);
        }
    }

    if(argc == 5){
        provided_time = atoi(argv[1]);
        simulation_time = provided_time * 60;

        available_runways = atoi(argv[2]);
        available_gates = atoi(argv[3]);
        available_tower_slots = atoi(argv[4]);

        if(available_runways <= 0 || available_gates <=0 || available_tower_slots <= 0){
            available_runways = DEFAULT_RUNWAYS;
            available_gates = DEFAULT_GATES;
            available_tower_slots = DEFAULT_TOWER_SLOTS;

            printf("Invalid quantity of resources. Each resource must have at least 1 quantity. Default simulation resources of %d runways, %d gates and %d towers slots.\n", available_runways, available_gates, available_tower_slots);
        }
    }

    if((argc > 2 && argc < 5) || argc > 5){
        provided_time = atoi(argv[1]);
        simulation_time = provided_time * 60;

        available_runways = DEFAULT_RUNWAYS;
        available_gates = DEFAULT_GATES;
        available_tower_slots = DEFAULT_TOWER_SLOTS;

        printf("Invalid quantity of resources. Each resource must have at least 1 quantity. Default simulation resources of %d runways, %d gates and %d towers slots.\n", available_runways, available_gates, available_tower_slots);
    }
}

void free_threads(pthread_t *flight_threads, Flight **flights){
    
    for(int i = 0; i < num_threads; i++){
        pthread_join(flight_threads[i], NULL);
        free(flights[i]);
    }

    free(flights);
    free(flight_threads);
}

void waiting_runway_queue(Flight *flight){
    pthread_mutex_lock(&runway_mutex);
    if(flight->type == INTERNATIONAL){
        international_flight_waiting_runway++;
    } else {
        national_flight_waiting_runway++;
    }
    pthread_mutex_unlock(&runway_mutex);
}

void waiting_tower_queue(Flight *flight){
    pthread_mutex_lock(&tower_mutex);
    if(flight->type == INTERNATIONAL){
        international_flight_waiting_tower++;
    } else {
        national_flight_waiting_tower++;
    }
    pthread_mutex_unlock(&tower_mutex);
}

void waiting_gate_queue(Flight *flight){
    pthread_mutex_lock(&gate_mutex);
    if(flight->type == INTERNATIONAL){
        international_flight_waiting_gate++;
    } else {
        national_flight_waiting_gate++;
    }
    pthread_mutex_unlock(&gate_mutex);
}

void request_runway(Flight *flight){
    pthread_mutex_lock(&runway_mutex);
    available_runways--;

    printf("%s flight %d took a runway! Available runways: %d\n", (flight->type == INTERNATIONAL ? "International" : "National"), flight->id, available_runways);
    
    if(flight->type == INTERNATIONAL){
        international_flight_waiting_runway--;
    } else {
        national_flight_waiting_runway--;
    }
    pthread_mutex_unlock(&runway_mutex);
}

void release_runway(Flight *flight){
    pthread_mutex_lock(&runway_mutex);
    available_runways++;
    
    printf("%s flight %d has released the runway. Available runways: %d\n", (flight->type == INTERNATIONAL ? "International" : "National"), flight->id, available_runways);
    pthread_mutex_unlock(&runway_mutex);

    sem_post(&runway_sem);
}

void request_gate(Flight *flight){
    pthread_mutex_lock(&gate_mutex);
    available_gates--;

    printf("%s flight %d took a gate! Available gates: %d\n", (flight->type == INTERNATIONAL ? "International" : "National"), flight->id, available_gates);

    if(flight->type == INTERNATIONAL){
        international_flight_waiting_gate--;
    } else {
        national_flight_waiting_gate--;
    }
    pthread_mutex_unlock(&gate_mutex);
}

void release_gate(Flight *flight){
    pthread_mutex_lock(&gate_mutex);
    available_gates++;
    
    printf("%s flight %d has released the gate. Available gates: %d\n", (flight->type == INTERNATIONAL ? "International" : "National"), flight->id, available_gates);
    pthread_mutex_unlock(&gate_mutex);
    
    sem_post(&gate_sem);
}

void request_tower(Flight *flight){
    pthread_mutex_lock(&tower_mutex);
    available_tower_slots--;
    
    printf("%s flight %d made contact with the tower. Available tower slots: %d\n", (flight->type == INTERNATIONAL ? "International" : "National"), flight->id, available_tower_slots);
    if(flight->type == INTERNATIONAL){
        international_flight_waiting_tower--;
    } else {
        national_flight_waiting_tower--;
    }
    pthread_mutex_unlock(&tower_mutex);

}

void release_tower(Flight *flight){
    pthread_mutex_lock(&tower_mutex);
    available_tower_slots++;
    
    printf("%s flight %d has released the tower! Available tower slots: %d\n", (flight->type == INTERNATIONAL ? "International" : "National"), flight->id, available_tower_slots);
    pthread_mutex_unlock(&tower_mutex);

    sem_post(&tower_sem);
}