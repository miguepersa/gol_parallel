#include "gol.h"

/* Funcion que recibe los hilos */
void* leer_archivo(void *arg)
{
    int i, j, c;
    hilo *esclavo = (hilo *)arg;
   	pthread_mutex_lock(&esclavo->mem->mutex);
    /* Lectura en orden */
    while(esclavo->mem->turno != esclavo->id)
    {
        pthread_cond_wait(&esclavo->mem->condicion, &esclavo->mem->mutex);
    }  
    /* Inicio region critica */
    for (i = esclavo->inicio; i < esclavo->fin; i++)
    {
        for(j = 0; j < esclavo->mem->ncol; j++)
        {
            c = fgetc(esclavo->mem->fp);
            if(c == '\n')
            {
                j--;
                continue;
            }
            esclavo->mem->tablero_actual[i][j] = c;
        }
    }
    esclavo->mem->turno++;
    /* Fin region critica */
   	pthread_mutex_unlock(&esclavo->mem->mutex);
	pthread_cond_broadcast(&esclavo->mem->condicion);
    juego(esclavo);
    return NULL;
}

/* Funcion para imprimir el tablero */
void imprimir_tablero(char **tablero, int nfilas, int ncol)
{
    int i, j;
    for (i = 0; i < nfilas; i++)
    {
        for (j = 0; j < ncol; j++)
        {
            if(tablero[i][j] == '1')
            {
                printf("*");
            }
            else
            {
                printf(" ");
            }
        }
        printf("\n");
    }
}

/* Funcion que calcula la cantidad de celulas vivas adyacentes */
int contar_vecinos_vivos(char **a, int r, int c, int nfilas, int ncol)
{
    int i, j, vecinos_vivos = 0;
    for (i = r - 1; i <= r + 1; i++)
    {
        for (j = c - 1; j <= c + 1; j++) 
        {
            if ( (i == r && j == c) || (i < 0 || j < 0) || (i >= nfilas || j >= ncol)) 
            {
                continue;
            }
            if (a[i][j] == '1') 
            {
                vecinos_vivos++;
            }
        }
    }
    return vecinos_vivos;
}

void juego(hilo *esclavo)
{
    int i, j, vecinos_vivos;
    for (i = esclavo->inicio; i < esclavo->fin; i++)
    {
        for (j = 0; j < esclavo->mem->ncol; j++) 
        {
            vecinos_vivos = contar_vecinos_vivos(esclavo->mem->tablero_actual, i, j, esclavo->mem->nfilas, esclavo->mem->ncol);
            if (esclavo->mem->tablero_actual[i][j] == '1' && (vecinos_vivos == 2 || vecinos_vivos == 3))
            {
                esclavo->mem->tablero_futuro[i][j] = '1';
            }
            else if (esclavo->mem->tablero_actual[i][j] == '0' && vecinos_vivos == 3) 
            {
                esclavo->mem->tablero_futuro[i][j] = '1';
            }
            else 
            {
                esclavo->mem->tablero_futuro[i][j] = '0';
            }
        }
    }
    imprimir_tablero(esclavo->mem->tablero_futuro, esclavo->mem->nfilas, esclavo->mem->ncol);
}