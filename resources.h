#ifndef RESOURCES_H
#define RESOURCES_H
#include <pthread.h>
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

int random_time();
void init_resources();
void destroy_resources();

void request_runway(Flight *flight, pthread_mutex_t *runway_mutex);
void release_runway(Flight *flight, pthread_mutex_t *runway_mutex);
void request_gate(Flight *flight, pthread_mutex_t *gate_mutex);
void release_gate(Flight *flight, pthread_mutex_t *gate_mutex);
void request_tower(Flight *flight, pthread_mutex_t *tower_mutex);
void release_tower(Flight *flight, pthread_mutex_t *tower_mutex);

#endif