#include "gol.h"

int main(int argc, char const *argv[])
{
    int i;
    int n_hilos;
    int n_generaciones;
    int n_visualizaciones;
    int fragmentos;
    char archivo[MAX_STRING];
    memoria_compartida *mem = NULL;
	pthread_t *threads; 						
    hilo *hilos;
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
    hilos = (hilo *)malloc(n_hilos * sizeof(hilo));

    /* Inicializar memoria compartida */
    mem = (memoria_compartida *) malloc(sizeof(memoria_compartida));
    pthread_mutex_init(&mem->mutex, NULL);
    pthread_cond_init(&mem->condicion, NULL);
    mem->turno = 0;
    mem->n_generaciones = n_generaciones;
    mem->n_visualizaciones = n_visualizaciones;

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

    /* Reservar memoria para los tableros */
    mem->tablero_actual = (char**) malloc(mem->nfilas * sizeof(char*));
    for(i = 0; i < mem->nfilas; i++)
    {
        mem->tablero_actual[i] = (char*) malloc(mem->ncol * sizeof(char));
    }
    mem->tablero_futuro = (char**) malloc(mem->nfilas * sizeof(char*));
    for(i = 0; i < mem->nfilas; i++)
    {
        mem->tablero_futuro[i] = (char*) malloc(mem->ncol * sizeof(char));
    }

    /* Crear hilos */
	for(i = 0; i < n_hilos; i++) 			
	{
		hilos[i].id = i;
		hilos[i].inicio = i * fragmentos;
		hilos[i].fin = (i+1) * fragmentos;
		hilos[i].mem = mem;
		pthread_create(&threads[i], NULL, leer_archivo, (void*) &hilos[i]);
	}

    /* Esperar por los hilos */
	for(i = 0; i < n_hilos; i++) 			
	{
		pthread_cancel(threads[i]);
		pthread_join(threads[i], &status);
	}

    /* Liberar memoria */
    for (i = 0; i < mem->nfilas; i++)
    {
        free(mem->tablero_actual[i]);
        free(mem->tablero_futuro[i]);
    }
    free(hilos);
    free(mem->tablero_actual);
    free(mem->tablero_futuro);
    fclose(mem->fp);
	pthread_mutex_destroy(&mem->mutex);
   	pthread_cond_destroy(&mem->condicion); 
    free(mem);
    return 0;
}