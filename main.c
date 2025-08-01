#include <pthread.h>
#include <stdlib.h>
#include <stdio.h>
#include "resources.h"

int main(){
    printf("Hello World!");

    Gate gate;

    gate.id = 1;
    gate.status = OCCUPIED;

    printf("%d", gate.id);
}