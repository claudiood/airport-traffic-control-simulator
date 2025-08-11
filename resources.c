#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "flight.h"
#include "resources.h"


short int international_flight_waiting_runway = 0;
short int national_flight_waiting_runway = 0;

short int international_flight_waiting_gate = 0;
short int national_flight_waiting_gate = 0;

short int international_flight_waiting_tower = 0;
short int national_flight_waiting_tower = 0;

short int available_runways = 3;
short int available_gates = 5;
short int available_tower_slots = 2;

pthread_mutex_t runway_mutex;
pthread_mutex_t tower_mutex;
pthread_mutex_t gate_mutex;

pthread_cond_t runway_cond;
pthread_cond_t tower_cond;
pthread_cond_t gate_cond;

void init_resources(){

    pthread_mutex_init(&runway_mutex, NULL);
    pthread_mutex_init(&tower_mutex, NULL);
    pthread_mutex_init(&gate_mutex, NULL);

    pthread_cond_init(&runway_cond, NULL);
    pthread_cond_init(&tower_cond, NULL);
    pthread_cond_init(&gate_cond, NULL);

    printf("Resources started.\n");
}

void destroy_resources(){
    pthread_mutex_destroy(&runway_mutex);
    pthread_mutex_destroy(&tower_mutex);
    pthread_mutex_destroy(&gate_mutex);

    pthread_cond_destroy(&runway_cond);
    pthread_cond_destroy(&tower_cond);
    pthread_cond_destroy(&gate_cond);

    printf("Resources destroyed.\n");
}

void request_runway(Flight *flight, pthread_mutex_t *runway_mutex){

    if(flight->type == INTERNATIONAL){
        international_flight_waiting_runway++;
    } else {
        national_flight_waiting_runway++;
    }

    while(available_runways == 0 || (flight->type == NATIONAL && flight->status == NORMAL && international_flight_waiting_runway > 0)){
        pthread_cond_wait(&runway_cond, runway_mutex);
    }

    available_runways--;
    if(flight->type == INTERNATIONAL){
        international_flight_waiting_runway--;
    } else {
        national_flight_waiting_runway--;
    }

    printf("%s flight %d took a runway! Available runways: %d\n", (flight->type == INTERNATIONAL ? "International" : "National"), flight->id, available_runways);
}

void release_runway(Flight *flight, pthread_mutex_t *runway_mutex){

    pthread_cond_signal(&runway_cond);
    
    printf("%s flight %d has released the runway. Available runways: %d\n", (flight->type == INTERNATIONAL ? "International" : "National"), flight->id, available_runways);
    pthread_mutex_unlock(runway_mutex);
    available_runways++;
}

void request_gate(Flight *flight, pthread_mutex_t *gate_mutex){

    if(flight->type == INTERNATIONAL){
        international_flight_waiting_gate++;
    } else {
        national_flight_waiting_gate++;
    }

    while(available_gates == 0 || (flight->type == NATIONAL && flight->status == NORMAL && international_flight_waiting_gate > 0)){
        pthread_cond_wait(&gate_cond, gate_mutex);
    }

    available_gates--;
    if(flight->type == INTERNATIONAL){
        international_flight_waiting_gate--;
    } else {
        national_flight_waiting_gate--;
    }

    printf("%s flight %d took a gate! Available gates: %d\n", (flight->type == INTERNATIONAL ? "International" : "National"), flight->id, available_gates);
}

void release_gate(Flight *flight, pthread_mutex_t *gate_mutex){

    pthread_cond_signal(&gate_cond);
    
    printf("%s flight %d has released the gate. Available gates: %d\n", (flight->type == INTERNATIONAL ? "International" : "National"), flight->id, available_gates);
    pthread_mutex_unlock(gate_mutex);
    available_gates++;
}

void request_tower(Flight *flight, pthread_mutex_t *tower_mutex){

    if(flight->type == INTERNATIONAL){
        international_flight_waiting_tower++;
    } else {
        national_flight_waiting_tower++;
    }

    while(available_tower_slots == 0 || (flight->type == NATIONAL && flight->status == NORMAL && international_flight_waiting_tower > 0)){
        pthread_cond_wait(&tower_cond, tower_mutex);
    }

    available_tower_slots--;
    if(flight->type == INTERNATIONAL){
        international_flight_waiting_tower--;
    } else {
        national_flight_waiting_tower--;
    }

    printf("%s flight %d made contact with the tower. Available tower slots: %d\n", (flight->type == INTERNATIONAL ? "International" : "National"), flight->id, available_tower_slots);
}

void release_tower(Flight *flight, pthread_mutex_t *tower_mutex){
    pthread_cond_signal(&tower_cond);
    
    printf("%s flight %d has released the tower! Available tower slots: %d\n", (flight->type == INTERNATIONAL ? "International" : "National"), flight->id, available_tower_slots);
    pthread_mutex_unlock(tower_mutex);
    available_tower_slots++;
}