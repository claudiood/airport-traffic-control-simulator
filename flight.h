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

typedef enum{
    NORMAL,
    CRITICAL
} Status;

typedef struct{
    int id;
    FlightType type;
    time_t created_At;
    Status status;
} Flight;

void *simulate_flight(void *arg);

#endif