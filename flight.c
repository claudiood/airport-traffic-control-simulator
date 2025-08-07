#include "flight.h"
#include "resources.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#define True 1

void *simulate_flight(void *arg){
    Flight *flight = (Flight*)arg;

    if(flight->type == INTERNATIONAL){

        printf("%s flight %d will attempt to land!\n", (flight->type == INTERNATIONAL ? "International" : "National"), flight->id);

        //landing
        while(True){
            if(pthread_mutex_trylock(&runway_mutex) == 0){
                if(pthread_mutex_trylock(&tower_mutex) == 0){
                        request_runway(flight, &runway_mutex);
                        sleep(random_time());
                        
                        request_tower(flight, &tower_mutex);
                        sleep(random_time());
                        
                        release_runway(flight, &runway_mutex);
                        sleep(random_time());
                        release_tower(flight, &tower_mutex);
                        
                        pthread_mutex_unlock(&runway_mutex);
                        pthread_mutex_unlock(&tower_mutex);   

                        break;
                } else {
                    pthread_mutex_unlock(&runway_mutex);
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
                    sleep(random_time());

                    request_tower(flight, &tower_mutex);
                    sleep(random_time());

                    release_gate(flight, &gate_mutex);
                    sleep(random_time());
                    release_tower(flight, &tower_mutex);

                    pthread_mutex_unlock(&gate_mutex);
                    pthread_mutex_unlock(&tower_mutex);

                    break;
                } else {
                    pthread_mutex_unlock(&gate_mutex);
                }
            } else {
                pthread_mutex_unlock(&gate_mutex);
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
                        sleep(random_time());

                        request_runway(flight, &runway_mutex);
                        sleep(random_time());
                        
                        request_tower(flight, &tower_mutex);
                        sleep(random_time());

                        release_gate(flight, &gate_mutex);
                        sleep(random_time());
                        release_runway(flight, &runway_mutex);
                        sleep(random_time());
                        release_tower(flight, &tower_mutex);
                        
                        pthread_mutex_unlock(&gate_mutex);
                        pthread_mutex_unlock(&runway_mutex);
                        pthread_mutex_unlock(&tower_mutex);

                        break;
                    } else {
                        pthread_mutex_unlock(&gate_mutex);
                        pthread_mutex_unlock(&runway_mutex);
                    }
                } else {
                    pthread_mutex_unlock(&gate_mutex);
                    sleep(1);
                }
            }
        }

        printf("%s flight %d took off!\n", (flight->type == INTERNATIONAL ? "International" : "National"), flight->id);
    } else {

        printf("%s flight %d will attempt to land!\n", (flight->type == INTERNATIONAL ? "International" : "National"), flight->id);

        //landing
        while(True){
            if(pthread_mutex_trylock(&tower_mutex) == 0){
                if(pthread_mutex_trylock(&runway_mutex) == 0){
                    request_tower(flight, &tower_mutex);
                    sleep(random_time());

                    request_runway(flight, &runway_mutex);
                    sleep(random_time());

                    release_tower(flight, &tower_mutex);
                    sleep(random_time());
                    release_runway(flight, &runway_mutex);

                    break;
                } else {
                    pthread_mutex_unlock(&tower_mutex);
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
                    sleep(random_time());

                    request_gate(flight, &gate_mutex);
                    sleep(random_time());

                    release_tower(flight, &tower_mutex);
                    sleep(random_time());
                    release_gate(flight, &gate_mutex);

                    pthread_mutex_unlock(&tower_mutex);
                    pthread_mutex_unlock(&gate_mutex);

                    break;
                } else {
                    pthread_mutex_unlock(&tower_mutex);
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
                        sleep(random_time());
    
                        request_gate(flight, &gate_mutex);
                        sleep(random_time());

                        request_runway(flight, &runway_mutex);
                        sleep(random_time());
    
                        release_tower(flight, &tower_mutex);
                        sleep(random_time());
                        release_gate(flight, &gate_mutex);
                        sleep(random_time());
                        release_runway(flight, &runway_mutex);
    
                        pthread_mutex_unlock(&tower_mutex);
                        pthread_mutex_unlock(&gate_mutex);
                        pthread_mutex_unlock(&runway_mutex);

                        break;
                    } else {
                        pthread_mutex_unlock(&tower_mutex);
                        pthread_mutex_unlock(&gate_mutex);
                    }
                } else {
                    pthread_mutex_unlock(&tower_mutex);
                }
            } else {
                sleep(1);
            }
        }

        printf("%s flight %d took off!\n", (flight->type == INTERNATIONAL ? "International" : "National"), flight->id);
    }

    free(flight);
    return NULL;
}