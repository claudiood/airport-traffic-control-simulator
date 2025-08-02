#include "flight.h"
#include "resources.h"

void init_resources(){

    pthread_mutex_init(&runway_mutex, NULL);
    pthread_mutex_init(&tower_mutex, NULL);
    pthread_mutex_init(&gate_mutex, NULL);

    pthread_cond_init(&runway_cond, NULL);
    pthread_cond_init(&tower_cond, NULL);
    pthread_cond_init(&gate_cond, NULL);

    printf('Resources started.');
}

void destroy_resources(){
    pthread_mutex_destroy(&runway_mutex);
    pthread_mutex_destroy(&tower_mutex);
    pthread_mutex_destroy(&gate_mutex);

    pthread_cond_destroy(&runway_cond);
    pthread_cond_destroy(&tower_cond);
    pthread_cond_destroy(&gate_cond);

    printf('Resources destroyed.');
}

void request_take_off(Flight *flight){

}
void request_land(Flight *flight){

}

void request_runway(Flight *flight){
    pthread_mutex_lock(&runway_mutex);

    if(flight->type == INTERNATIONAL){
        international_flight_waiting_runway++;
    } else {
        national_flight_waiting_runway++;
    }

    while(availabel_runways == 0 || (flight->type == NATIONAL && international_flight_waiting_runway > 0)){
        pthread_cond_wait(&runway_cond, &runway_mutex);
    }

    availabel_runways--;
    if(flight->type == INTERNATIONAL){
        international_flight_waiting_runway--;
    } else {
        national_flight_waiting_runway--;
    }

    printf("%s Flight %d took a runway! Availabel runways: %d", (flight->type == INTERNATIONAL ? "International" : "National"), flight->id, availabel_runways);
    pthread_mutex_unlock(&runway_mutex);
}

void release_runway(Flight *flight){

}
void request_gate(Flight *flight){

}
void release_gate(Flight *flight){

}
void request_tower(Flight *flight){

}
void release_tower(Flight *flight){

}