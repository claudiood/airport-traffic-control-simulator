#include "flight.h"
#include "resources.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void *simulate_flight(void *arg){
    Flight *flight = (Flight*)arg;

    if(flight->type == INTERNATIONAL){

        printf("%s flight %d will attempt to land!\n", (flight->type == INTERNATIONAL ? "International" : "National"), flight->id);

        //landing
        sleep(random_time());
        request_runway(flight);
        sleep(random_time());
        request_tower(flight);
        sleep(random_time());
        release_tower(flight);
        release_runway(flight);

        printf("%s flight %d has landed!\n", (flight->type == INTERNATIONAL ? "International" : "National"), flight->id);
        
        //landing gate
        sleep(random_time());
        request_gate(flight);
        sleep(random_time());
        request_tower(flight);
        sleep(random_time());

        printf("%s flight %d will take-off!\n", (flight->type == INTERNATIONAL ? "International" : "National"), flight->id);

        //taking off
        request_gate(flight);
        sleep(random_time());
        request_runway(flight);
        sleep(random_time());
        request_tower(flight);
        sleep(random_time());

        printf("%s flight %d took off!\n", (flight->type == INTERNATIONAL ? "International" : "National"), flight->id);
    } else {

        printf("%s flight %d will attempt to land!\n", (flight->type == INTERNATIONAL ? "International" : "National"), flight->id);

        //landing
        request_tower(flight);
        sleep(random_time());
        request_runway(flight);
        sleep(random_time());
        release_tower(flight);
        release_runway(flight);

        printf("%s flight %d has landed!\n", (flight->type == INTERNATIONAL ? "International" : "National"), flight->id);

        //landing gate
        request_tower(flight);
        sleep(random_time());
        request_gate(flight);
        sleep(random_time());
        release_tower(flight);
        release_gate(flight);

        printf("%s flight %d will take-off!\n", (flight->type == INTERNATIONAL ? "International" : "National"), flight->id);

        //taking off
        request_tower(flight);
        sleep(random_time());
        request_gate(flight);
        sleep(random_time());
        request_runway(flight);
        sleep(random_time());
        release_tower(flight);
        release_gate(flight);
        release_runway(flight);

        printf("%s flight %d took off!\n", (flight->type == INTERNATIONAL ? "International" : "National"), flight->id);
    }

    free(flight);
    return NULL;
}