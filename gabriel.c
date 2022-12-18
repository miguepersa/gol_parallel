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

typedef struct
{
    FILE *fp;
    int **tablero;
    int nfilas;
    int ncol;
    pthread_mutex_t mutex;
}memoria_compartida;

void* leer_archivo(void *arg);


int main(int argc, char const *argv[])
{
    int n_hilos;
    int n_generaciones;
    int n_visualizaciones;
    char archivo[MAX_STRING];
    memoria_compartida *mem = NULL;
	pthread_t *hilos; 						

    /* Validar argumentos */
    if (argc != 5)
    {
    	fprintf(stderr, "Sintaxis errada, por favor usar: %s <n_hilos> <n_generaciones> <n_visualizacion> <archivo>\n", argv[0]);		
        return 1;
    }    

    /* Se reciben los argumentos en el programa */
    n_hilos = atoi(argv[1]);
    n_generaciones = atoi(argv[2]);
    n_visualizaciones = atoi(argv[3]);
    strcpy(archivo, argv[4]);

    /* Reservar memoria para hilos */
    hilos = (pthread_t *)malloc(n_hilos * sizeof(pthread_t));

    /* Inicializar memoria compartida */
    mem = (memoria_compartida *) malloc(sizeof(memoria_compartida));
    pthread_mutex_init(&mem->mutex, NULL);
    
    /* Abrir archivo de entrada */
    if( (mem->fp = fopen(archivo, "r")) == NULL)
    {
    	pthread_mutex_destroy(&mem->mutex);
        free(mem);
        return 1;
    }

    /* Leer dimensiones del tablero */
    fscanf(mem->fp, "%d %d\n", &mem->nfilas, &mem->ncol);

    /* Reservar memoria para el tablero */
    mem->tablero = (int**) malloc(mem->nfilas * sizeof(int*));
    for(int i = 0; i < mem->nfilas; i++)
    {
        mem->tablero[i] = (int*) malloc(mem->ncol * sizeof(int));
    }

    /* Leer archivo */
    //pthread_create(&Worker[j], NULL, thread_Pool, (void*) Arg[j]);


    /* Liberar memoria */
    for (int i = 0; i < mem->nfilas; i++)
    {
        free(mem->tablero[i]);
    }
    free(hilos);
    free(mem->tablero);
    fclose(mem->fp);
	pthread_mutex_destroy(&mem->mutex);
    free(mem);
    return 0;
}

void* leer_archivo(void *arg)
{

}