#ifndef GOL_H
#define GOL_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <sys/types.h>
#include <pthread.h>

#define MAX_STRING 256
#define MAX_HILOS 256

typedef struct
{
    FILE *fp;
    char **tablero_actual;
    char **tablero_futuro;
    int nfilas;
    int ncol;
    int turno;
    int n_generaciones;
    int n_visualizaciones;
    pthread_mutex_t mutex;
	pthread_cond_t condicion;
}memoria_compartida;

typedef struct
{
    int id;
    int inicio;
    int fin;
    memoria_compartida *mem;
}hilo;

void* leer_archivo(void *);
void imprimir_tablero(char **, int, int);
void juego(hilo *);
int contar_vecinos_vivos(char **, int, int, int, int);

#endif