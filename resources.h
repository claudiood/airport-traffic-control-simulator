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

extern short int international_flight_waiting_runway;
extern short int national_flight_waiting_runway;
extern short int availabel_runways;
extern short int availabel_gates;
extern short int availabel_towers;

void request_take_off(Flight *flight);
void request_land(Flight *flight);
void request_runway(Flight *flight);
void release_runway(Flight *flight);
void request_gate(Flight *flight);
void release_gate(Flight *flight);
void request_tower(Flight *flight);
void release_tower(Flight *flight);

#endif