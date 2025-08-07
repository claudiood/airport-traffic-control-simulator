#ifndef FLIGHT_H
#define FLIGHT_H
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <time.h>

typedef enum{
    NATIONAL,
    INTERNATIONAL
} FlightType;

typedef struct{
    int id;
    FlightType type;
} Flight;

void *simulate_flight(void *arg);

#endif