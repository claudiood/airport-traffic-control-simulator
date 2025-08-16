#ifndef RESOURCES_H
#define RESOURCES_H
#include <pthread.h>
#include <semaphore.h>
#include "flight.h"

extern short int international_flight_waiting_runway;
extern short int national_flight_waiting_runway;

extern short int international_flight_waiting_gate;
extern short int national_flight_waiting_gate;

extern short int international_flight_waiting_tower;
extern short int national_flight_waiting_tower;

extern short int available_runways;
extern short int available_gates;
extern short int available_tower_slots;

extern pthread_mutex_t runway_mutex;
extern pthread_mutex_t tower_mutex;
extern pthread_mutex_t gate_mutex;

extern pthread_cond_t runway_cond;
extern pthread_cond_t tower_cond;
extern pthread_cond_t gate_cond;

extern sem_t runway_sem;
extern sem_t tower_sem;
extern sem_t gate_sem;

void init_resources();
void destroy_resources();

void request_runway(Flight *flight);
void release_runway(Flight *flight);
void request_gate(Flight *flight);
void release_gate(Flight *flight);
void request_tower(Flight *flight);
void release_tower(Flight *flight);

#endif