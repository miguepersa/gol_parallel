#ifndef FUNCIONES_H
#define FUNCIONES_H

#include <stdio.h>
#include <stdlib.h>

#define MAX_COL 128
#define MAX_FIL 128
#define MAX_ARR 100

/* Estructura de datos utilizado para cada celula*/
typedef struct {
    int estado;
    int n_estado;
}celula;

/* Funcion que procesa el archivo de entrada */
void procesar_archivo(celula m[][MAX_COL], char *, int*, int*);

/* Funcion que muestra el estado actual del mapa */
void print_gen(celula m[][MAX_COL], int, int);

/* Funcion utilizada para contar los vecinos de una celula */
int count_vecinos(celula[][MAX_COL], int, int, int, int);

#endif