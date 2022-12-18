#ifndef GOLH_H
#define GOLH_H

#include <pthread.h>
#include <errno.h>

#include "funciones.h"

typedef struct{
    int p;
    int q;
    int n;
}arg;

/* Funcion principal del juego que instancia los hilos */
void golh(celula[][MAX_COL], int[][MAX_COL], int, int, int, int, int);

/* Runner de los hilos */
void *run(void*);

/* Funcion que cuenta los vecinos de las celulas y genera el estado siguiente */
void gen_next_state(int, int);

/* Funcion que actualiza el estado de las celulas al siguiente */
void update_state(int, int);

/* Funcion que devuelve si ya todos los estados fueron generados */
int update();


#endif