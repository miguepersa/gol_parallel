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

/* Mapa del juego */
celula mapa[MAX_FIL][MAX_COL];

int flag[MAX_FIL][MAX_COL];

/* Bloqueador mutex */
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

/* Numero de filas y columnas del mapa */
int filas = 0;
int cols = 0;

/* Funcion principal del juego que instancia los hilos */
void golh(int, int, int, int, int);

/* Runner de los hilos */
void *run(arg*);

/* Funcion que cuenta los vecinos de las celulas y genera el estado siguiente */
void gen_next_state(int, int);

/* Funcion que actualiza el estado de las celulas al siguiente */
void update_state(int, int);

/* Funcion que devuelve si ya todos los estados fueron generados */
int update();


#endif