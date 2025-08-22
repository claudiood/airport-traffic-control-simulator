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

        waiting_runway_queue(flight);
        waiting_tower_queue(flight);

        //landing
        while(True){
            if(sem_trywait(&runway_sem) == 0){
                if(sem_trywait(&tower_sem) == 0){
                        request_runway(flight);
                        sleep(1);
                        request_tower(flight);
                        
                        release_runway(flight);
                        release_tower(flight);   

                        break;
                } else {
                    sem_post(&runway_sem);
                    sleep(1);
                }
            } else {
                sleep(1);
            }
    }      
        
        printf("%s flight %d has landed!\n", (flight->type == INTERNATIONAL ? "International" : "National"), flight->id);
        
        waiting_gate_queue(flight);
        waiting_tower_queue(flight);

        //landing gate
        while(True){
            if(sem_trywait(&gate_sem) == 0){
                if(sem_trywait(&tower_sem) == 0){
                    request_gate(flight);
                    sleep(1);
                    request_tower(flight);
                    sleep(1);

                    release_tower(flight);
                    sleep(1);
                    release_gate(flight);

                    break;
                } else {
                    sem_post(&gate_sem);
                    sleep(1);
                }
            } else {
                sleep(1);
            }
        }

        printf("%s flight %d will take-off!\n", (flight->type == INTERNATIONAL ? "International" : "National"), flight->id);

        waiting_gate_queue(flight);
        waiting_runway_queue(flight);
        waiting_tower_queue(flight);

        //taking off
        while(True){
            if(sem_trywait(&gate_sem) == 0){
                if(sem_trywait(&runway_sem) == 0){
                    if(sem_trywait(&tower_sem) == 0){

                        request_gate(flight);
                        sleep(1);
                        request_runway(flight);
                        request_tower(flight);
                        sleep(1);

                        release_tower(flight);
                        release_gate(flight);
                        release_runway(flight);

                        break;
                    } else {
                        sem_post(&gate_sem);
                        sem_post(&runway_sem);
                        sleep(1);
                    }
                } else {
                    sem_post(&gate_sem);
                    sleep(1);
                }
            } else {
                sleep(1);
            }
        }

        printf("%s flight %d took off!\n", (flight->type == INTERNATIONAL ? "International" : "National"), flight->id);
    } else {

        printf("%s flight %d will attempt to land!\n", (flight->type == INTERNATIONAL ? "International" : "National"), flight->id);

        waiting_tower_queue(flight);
        waiting_runway_queue(flight);
        
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
                pthread_mutex_lock(&summary);
                national_crashes++;
                pthread_mutex_unlock(&summary);
                pthread_exit((void*)1);
            }

            if(flight->status == NORMAL && international_flight_waiting_runway > 0 && international_flight_waiting_tower > 0){
                sleep(1);
                continue;
            }

            if(sem_trywait(&tower_sem) == 0){
                if(sem_trywait(&runway_sem) == 0){
                    request_tower(flight);
                    request_runway(flight);

                    release_tower(flight);
                    release_runway(flight);

                    break;
                } else {
                    sem_post(&tower_sem);
                    sleep(1);
                }
            } else {
                sleep(1);
            }
        }

        printf("%s flight %d has landed!\n", (flight->type == INTERNATIONAL ? "International" : "National"), flight->id);

        waiting_tower_queue(flight);
        waiting_gate_queue(flight);

        while(True){

            if(flight->status == NORMAL && international_flight_waiting_tower > 0 && international_flight_waiting_gate > 0){
                sleep(1);
                continue;
            }

            if(sem_trywait(&tower_sem) == 0){
                if(sem_trywait(&gate_sem) == 0){
                    request_tower(flight);
                    request_gate(flight);

                    release_tower(flight);
                    sleep(1);
                    release_gate(flight);

                    break;
                } else {
                    sem_post(&tower_sem);
                    sleep(1);
                }
            } else {
                sleep(1);
            }
        }

        printf("%s flight %d will take-off!\n", (flight->type == INTERNATIONAL ? "International" : "National"), flight->id);

        waiting_tower_queue(flight);
        waiting_runway_queue(flight);
        waiting_gate_queue(flight);

        //taking off
        while(True){

            if(flight->status == NORMAL && international_flight_waiting_tower > 0  && international_flight_waiting_gate > 0 && international_flight_waiting_runway > 0){
                sleep(1);
                continue;
            }

            if(sem_trywait(&tower_sem) == 0){
                if(sem_trywait(&gate_sem) == 0){
                    if(sem_trywait(&runway_sem) == 0){
                        request_tower(flight);
                        request_gate(flight);
                        request_runway(flight);
                        sleep(1);
    
                        release_tower(flight);
                        sleep(1);
                        release_gate(flight);
                        release_runway(flight);

                        break;
                    } else {
                        sem_post(&tower_sem);
                        sem_post(&gate_sem);
                        sleep(1);
                    }
                } else {
                    sem_post(&tower_sem);
                    sleep(1);
                }
            } else {
                sleep(1);
            }
        }

        printf("%s flight %d took off!\n", (flight->type == INTERNATIONAL ? "International" : "National"), flight->id);
    }

    pthread_mutex_lock(&summary);
    if(flight->type == INTERNATIONAL){
        international_sucessess++;
    } else {
        national_sucessess++;
    }
    pthread_mutex_unlock(&summary);
    
    return NULL;
}