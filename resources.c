#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "flight.h"
#include "resources.h"

short int international_sucessess = 0;
short int national_sucessess = 0;
short int national_crashes = 0;

short int international_flight_waiting_runway = 0;
short int national_flight_waiting_runway = 0;

short int international_flight_waiting_gate = 0;
short int national_flight_waiting_gate = 0;

short int international_flight_waiting_tower = 0;
short int national_flight_waiting_tower = 0;

short int available_runways = 3;
short int available_gates = 5;
short int available_tower_slots = 2;

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

    sem_init(&runway_sem, 0, 3);
    sem_init(&tower_sem, 0, 2);
    sem_init(&gate_sem, 0, 5);

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
    sem_post(&runway_sem);
    
    pthread_mutex_lock(&runway_mutex);
    available_runways++;

    printf("%s flight %d has released the runway. Available runways: %d\n", (flight->type == INTERNATIONAL ? "International" : "National"), flight->id, available_runways);
    pthread_mutex_unlock(&runway_mutex);
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
    sem_post(&gate_sem);
    
    pthread_mutex_lock(&gate_mutex);
    available_gates++;

    printf("%s flight %d has released the gate. Available gates: %d\n", (flight->type == INTERNATIONAL ? "International" : "National"), flight->id, available_gates);
    pthread_mutex_unlock(&gate_mutex);
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
    sem_post(&tower_sem);
    
    pthread_mutex_lock(&tower_mutex);
    available_tower_slots++;

    printf("%s flight %d has released the tower! Available tower slots: %d\n", (flight->type == INTERNATIONAL ? "International" : "National"), flight->id, available_tower_slots);
    pthread_mutex_unlock(&tower_mutex);
}