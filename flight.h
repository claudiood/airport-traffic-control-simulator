#ifndef FLIGHT_H
#define FLIGHT_H

typedef enum{
    NATIONAL,
    INTERNATIONAL
} FlightType;

typedef enum{
    WAITING,
    COMPLETED
} State;

typedef struct{
    int id;
    FlightType type;
} Flight;

#endif