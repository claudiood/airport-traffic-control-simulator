#ifndef RESOURCES_H
#define RESOURCES_H
#include <pthread.h>
#include "flight.h"

typedef enum{
    OCCUPIED,
    FREE
} Status;

typedef struct{
    int id;
    Status status;
} Gate;

typedef struct{
    int id;
    Status status;
} Runway;

extern short int international_flight_waiting_runway = 0;
extern short int national_flight_waiting_runway = 0;

extern short int international_flight_waiting_gate = 0;
extern short int national_flight_waiting_gate = 0;

extern short int international_flight_waiting_tower = 0;
extern short int national_flight_waiting_tower = 0;

extern short int available_runways = 3;
extern short int available_gates = 5;
extern short int available_towers = 1;

extern pthread_mutex_t runway_mutex;
extern pthread_mutex_t tower_mutex;
extern pthread_mutex_t gate_mutex;

extern pthread_cond_t runway_cond;
extern pthread_cond_t tower_cond;
extern pthread_cond_t gate_cond;

void init_resources();
void destroy_resources();

void request_take_off(Flight *flight);
void request_land(Flight *flight);
void request_runway(Flight *flight);
void release_runway(Flight *flight);
void request_gate(Flight *flight);
void release_gate(Flight *flight);
void request_tower(Flight *flight);
void release_tower(Flight *flight);

#endif