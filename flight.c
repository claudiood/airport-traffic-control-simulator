#include "flight.h"
#include "resources.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define True 1
#define CRITICAL_TIME 60
#define CRASH_TIME 90

void *simulate_flight(void *arg){
    Flight *flight = (Flight*)arg;

    time_t current_time;
    double elapsed_time;

    if(flight->type == INTERNATIONAL){

        printf("%s flight %d will attempt to land!\n", (flight->type == INTERNATIONAL ? "International" : "National"), flight->id);

        //landing
        while(True){
            if(pthread_mutex_trylock(&runway_mutex) == 0){
                if(pthread_mutex_trylock(&tower_mutex) == 0){
                        request_runway(flight, &runway_mutex);
                        sleep(1);
                        request_tower(flight, &tower_mutex);
                        
                        release_runway(flight, &runway_mutex);
                        release_tower(flight, &tower_mutex);
                        
                        pthread_mutex_unlock(&runway_mutex);
                        pthread_mutex_unlock(&tower_mutex);   

                        break;
                } else {
                    pthread_mutex_unlock(&runway_mutex);
                    sleep(1);
                }
            } else {
                sleep(1);
            }
    }      
        
        printf("%s flight %d has landed!\n", (flight->type == INTERNATIONAL ? "International" : "National"), flight->id);
        
        //landing gate
        while(True){
            if(pthread_mutex_trylock(&gate_mutex) == 0){
                if(pthread_mutex_trylock(&tower_mutex) == 0){
                    request_gate(flight, &gate_mutex);
                    sleep(1);
                    request_tower(flight, &tower_mutex);
                    sleep(1);

                    release_tower(flight, &tower_mutex);
                    sleep(1);
                    release_gate(flight, &gate_mutex);

                    pthread_mutex_unlock(&gate_mutex);
                    pthread_mutex_unlock(&tower_mutex);

                    break;
                } else {
                    pthread_mutex_unlock(&gate_mutex);
                    sleep(1);
                }
            } else {
                sleep(1);
            }
        }

        printf("%s flight %d will take-off!\n", (flight->type == INTERNATIONAL ? "International" : "National"), flight->id);

        //taking off
        while(True){
            if(pthread_mutex_trylock(&gate_mutex) == 0){
                if(pthread_mutex_trylock(&runway_mutex) == 0){
                    if(pthread_mutex_trylock(&tower_mutex) == 0){

                        request_gate(flight, &gate_mutex);
                        sleep(1);
                        request_runway(flight, &runway_mutex);
                        request_tower(flight, &tower_mutex);
                        sleep(1);

                        release_tower(flight, &tower_mutex);
                        release_gate(flight, &gate_mutex);
                        release_runway(flight, &runway_mutex);
                        
                        pthread_mutex_unlock(&gate_mutex);
                        pthread_mutex_unlock(&runway_mutex);
                        pthread_mutex_unlock(&tower_mutex);

                        break;
                    } else {
                        pthread_mutex_unlock(&gate_mutex);
                        pthread_mutex_unlock(&runway_mutex);
                        sleep(1);
                    }
                } else {
                    pthread_mutex_unlock(&gate_mutex);
                    sleep(1);
                }
            } else {
                sleep(1);
            }
        }

        printf("%s flight %d took off!\n", (flight->type == INTERNATIONAL ? "International" : "National"), flight->id);
    } else {

        printf("%s flight %d will attempt to land!\n", (flight->type == INTERNATIONAL ? "International" : "National"), flight->id);

        
        //landing
        while(True){
            current_time = time(NULL);
            elapsed_time = difftime(current_time, flight->created_At);

            if(elapsed_time > CRITICAL_TIME && flight->status == NORMAL){
                flight->status = CRITICAL;
                printf("%s flight %d is in a critical situation!\n", (flight->type == INTERNATIONAL ? "International" : "National"), flight->id);
            }

            if(elapsed_time > CRASH_TIME){
                printf("%s flight %d crashed because of the waiting time.\n", (flight->type == INTERNATIONAL ? "International" : "National"), flight->id);
                pthread_exit((void*)1);
            }

            if(pthread_mutex_trylock(&tower_mutex) == 0){
                if(pthread_mutex_trylock(&runway_mutex) == 0){
                    request_tower(flight, &tower_mutex);
                    request_runway(flight, &runway_mutex);

                    release_tower(flight, &tower_mutex);
                    release_runway(flight, &runway_mutex);

                    break;
                } else {
                    pthread_mutex_unlock(&tower_mutex);
                    sleep(1);
                }
            } else {
                sleep(1);
            }
        }

        printf("%s flight %d has landed!\n", (flight->type == INTERNATIONAL ? "International" : "National"), flight->id);

        while(True){
            if(pthread_mutex_trylock(&tower_mutex) == 0){
                if(pthread_mutex_trylock(&gate_mutex) == 0){
                    request_tower(flight, &tower_mutex);
                    request_gate(flight, &gate_mutex);

                    release_tower(flight, &tower_mutex);
                    sleep(1);
                    release_gate(flight, &gate_mutex);

                    pthread_mutex_unlock(&tower_mutex);
                    pthread_mutex_unlock(&gate_mutex);

                    break;
                } else {
                    pthread_mutex_unlock(&tower_mutex);
                    sleep(1);
                }
            } else {
                sleep(1);
            }
        }

        printf("%s flight %d will take-off!\n", (flight->type == INTERNATIONAL ? "International" : "National"), flight->id);

        //taking off
        while(True){
            if(pthread_mutex_trylock(&tower_mutex) == 0){
                if(pthread_mutex_trylock(&gate_mutex) == 0){
                    if(pthread_mutex_trylock(&runway_mutex) == 0){
                        request_tower(flight, &tower_mutex);
                        request_gate(flight, &gate_mutex);
                        request_runway(flight, &runway_mutex);
    
                        release_tower(flight, &tower_mutex);
                        sleep(1);
                        release_gate(flight, &gate_mutex);
                        release_runway(flight, &runway_mutex);
    
                        pthread_mutex_unlock(&tower_mutex);
                        pthread_mutex_unlock(&gate_mutex);
                        pthread_mutex_unlock(&runway_mutex);

                        break;
                    } else {
                        pthread_mutex_unlock(&tower_mutex);
                        pthread_mutex_unlock(&gate_mutex);
                        sleep(1);
                    }
                } else {
                    pthread_mutex_unlock(&tower_mutex);
                    sleep(1);
                }
            } else {
                sleep(1);
            }
        }

        printf("%s flight %d took off!\n", (flight->type == INTERNATIONAL ? "International" : "National"), flight->id);
    }

    return NULL;
}