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

    while(available_runways == 0 || (flight->type == NATIONAL && international_flight_waiting_runway > 0)){
        pthread_cond_wait(&runway_cond, &runway_mutex);
    }

    available_runways--;
    if(flight->type == INTERNATIONAL){
        international_flight_waiting_runway--;
    } else {
        national_flight_waiting_runway--;
    }

    printf("%s flight %d took a runway! Available runways: %d", (flight->type == INTERNATIONAL ? "International" : "National"), flight->id, available_runways);
    pthread_mutex_unlock(&runway_mutex);
}

void release_runway(Flight *flight){
    pthread_mutex_lock(&runway_mutex);

    available_runways++;
    pthread_cond_signal(&runway_cond);

    printf("%s flight %d has released the runway. Available runways: %d", (flight->type == INTERNATIONAL ? "International" : "National"), flight->id, available_runways);
    pthread_mutex_unlock(&runway_mutex);
}

void request_gate(Flight *flight){
    pthread_mutex_lock(&gate_mutex);

    if(flight->type == INTERNATIONAL){
        international_flight_waiting_gate++;
    } else {
        national_flight_waiting_gate++;
    }

    while(available_gates == 0 || (flight->type == NATIONAL && international_flight_waiting_gate > 0)){
        pthread_cond_wait(&gate_cond, &gate_mutex);
    }

    available_gates--;
    if(flight->type == INTERNATIONAL){
        international_flight_waiting_gate--;
    } else {
        national_flight_waiting_gate--;
    }

    printf("%s flight %d took a gate! Available gates: %d", (flight->type == INTERNATIONAL ? "International" : "National"), flight->id, available_gates);
    pthread_mutex_unlock(&gate_mutex);
}

void release_gate(Flight *flight){
    pthread_mutex_lock(&gate_mutex);

    available_gates++;
    pthread_cond_signal(&gate_cond);

    printf("%s flight %d has released the gate. Available gates: %d", (flight->type == INTERNATIONAL ? "International" : "National"), flight->id, available_gates);
    pthread_mutex_unlock(&gate_mutex);
}

void request_tower(Flight *flight){
    pthread_mutex_lock(&tower_mutex);

    if(flight->type == INTERNATIONAL){
        international_flight_waiting_tower++;
    } else {
        national_flight_waiting_tower++;
    }

    while(available_towers == 0 || (flight->type == NATIONAL && international_flight_waiting_tower > 0)){
        pthread_cond_wait(&tower_cond, &tower_mutex);
    }

    available_towers--;
    if(flight->type == INTERNATIONAL){
        international_flight_waiting_tower--;
    } else {
        national_flight_waiting_tower--;
    }

    printf("%s flight %d made contact with the tower. Availabel towers: %d", (flight->type == INTERNATIONAL ? "International" : "National"), flight->id, available_towers);

    pthread_mutex_unlock(&tower_mutex);
}

void release_tower(Flight *flight){

}