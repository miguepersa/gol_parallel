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
    char **tablero;
    int nfilas;
    int ncol;
    int turno;
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

void* leer_archivo(void *arg);
void imprimir_tablero(memoria_compartida *mem);

int main(int argc, char const *argv[])
{
    int n_hilos;
    int n_generaciones;
    int n_visualizaciones;
    int fragmentos;
    char archivo[MAX_STRING];
    memoria_compartida *mem = NULL;
	pthread_t *threads; 						
    hilo *hilos[MAX_HILOS];
	void* status;

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
    threads = (pthread_t *)malloc(n_hilos * sizeof(pthread_t));
//    hilos = (hilo *)malloc(n_hilos * sizeof(hilo));

    /* Inicializar memoria compartida */
    mem = (memoria_compartida *) malloc(sizeof(memoria_compartida));
    pthread_mutex_init(&mem->mutex, NULL);
    pthread_cond_init(&mem->condicion, NULL);
    mem->turno = 0;

    /* Abrir archivo de entrada */
    if( (mem->fp = fopen(archivo, "r")) == NULL)
    {
    	pthread_mutex_destroy(&mem->mutex);
        free(mem);
        return 1;
    }

    /* Leer dimensiones del tablero */
    fscanf(mem->fp, "%d %d\n", &mem->nfilas, &mem->ncol);

    /* Calcular cuantas filas corresponden por hilo */
    fragmentos = mem->nfilas / n_hilos;

    /* Reservar memoria para el tablero */
    mem->tablero = (char**) malloc(mem->nfilas * sizeof(char*));
    for(int i = 0; i < mem->nfilas; i++)
    {
        mem->tablero[i] = (char*) malloc(mem->ncol * sizeof(char));
    }

    /* Leer archivo */
	for(int i = 0; i < n_hilos; i++) 			
	{
		hilos[i] = (hilo*) malloc(sizeof(hilo));
		hilos[i]->id = i;
		hilos[i]->inicio = i * fragmentos;
		hilos[i]->fin = (i+1) * fragmentos;
		hilos[i]->mem = mem;
		pthread_create(&threads[i], NULL, leer_archivo, (void*) hilos[i]);
	}

	for(int i = 0; i < n_hilos; i++) 			
	{
		//free(hilos[i]);
		pthread_cancel(threads[i]);
		pthread_join(threads[i], &status);
	}

    imprimir_tablero(mem);

    /* Liberar memoria */
    for (int i = 0; i < mem->nfilas; i++)
    {
        free(mem->tablero[i]);
    }
    //free(hilos);
    free(mem->tablero);
    fclose(mem->fp);
	pthread_mutex_destroy(&mem->mutex);
   	pthread_cond_destroy(&mem->condicion); 
    free(mem);
    return 0;
}

void* leer_archivo(void *arg)
{
    hilo *esclavo = (hilo *)arg;
   	pthread_mutex_lock(&esclavo->mem->mutex);
    while(esclavo->mem->turno != esclavo->id)
    {
        pthread_cond_wait(&esclavo->mem->condicion, &esclavo->mem->mutex);
    }  
    for (int i = esclavo->inicio; i < esclavo->fin; i++)
    {
        for(int j = 0; j < esclavo->mem->ncol; j++)
        {
            int c = fgetc(esclavo->mem->fp);
            if(c == '\n')
            {
                j--;
                continue;
            }
            esclavo->mem->tablero[i][j] = c;
        }
    }
    esclavo->mem->turno++;
   	pthread_mutex_unlock(&esclavo->mem->mutex);
	pthread_cond_broadcast(&esclavo->mem->condicion);
}

void imprimir_tablero(memoria_compartida *mem)
{
    for (int i = 0; i < mem->nfilas; i++)
    {
        for (int j = 0; j < mem->ncol; j++)
        {
            //printf("%c", mem->tablero[i][j]);
            if(mem->tablero[i][j] == '1')
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