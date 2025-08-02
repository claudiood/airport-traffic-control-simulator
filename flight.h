#ifndef FLIGHT_H
#define FLIGHT_H

typedef enum{
    NATIONAL,
    INTERNATIONAL
} FlightType;

typedef enum{
    READY,
    WAITING,
    ARRIVING,
    COMPLETED
} State;

typedef struct{
    int id;
    FlightType type;
    State state;
} Flight;

void *simulate_flight(void *arg);

#endif