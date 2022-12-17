#include "golh.h"

void golh(int nfilas, int ncols, int nhilos, int ngens, int nvis)
{
    /* Arreglo de identificadores de hilos */
    pthread_t hilos[nhilos];

    /* Variable de iteracion */
    int i, j;

    /* Variable que se utiliza para asignar filas a hilos */
    int k = nfilas / nhilos;
    
    /* Variables que definen los limites de los hilos */
    int p = 0;
    int q = 0;

    /* Variable donde almacenan los argumentos que recibe el runner */
    arg args[nhilos];

    /* Numero de filas y columnas del mapa */
    filas = nfilas;
    cols = ncols;

    for (i = 0; i < nfilas; i++)
    {
        for (j = 0; j < ncols; j++)
        {
            flag[i][j] = 0;
        }
    }
    

    /* Creacion de hilos */
    for (j = 0; j < ngens; j = j+nvis)
    {
        for (i = 0; i < nhilos; i++)
        {
            if (p + k >= nfilas)
            {
                q = nfilas;
            } else 
            {
                q = q+k;
            }

            args[i].p = p;
            args[i].q = q;

            if (pthread_create(&hilos[i], NULL, run, (void*) &args[i]) != 0)
            {
                printf("Error al crear hilo %d: %s\n", i, strerror(errno));
            }

            p = p + k;
        }
    }
    
    free(args);
    return;
}

void gen_next_state(int i, int j)
{
    /* Numero de vecinos de la celula en la posicion i,j */
    int vecinos = count_vecinos(mapa, i, j, filas, cols);

    if (mapa[i][j].estado == 0)
    {
        if (vecinos == 3)
        {
            mapa[i+1][j+1].n_estado = 1;
        } else
        {
            mapa[i+1][j+1].n_estado = 0;
        }
    } else if (mapa[i][j].estado == 1)
    {
        if (vecinos == 2 || vecinos == 3)
        {
            mapa[i+1][j+1].n_estado = 1;
        } else 
        {
            mapa[i+1][j+1].n_estado = 0;
        }
    }
}

void update_state(int i, int j)
{
    mapa[i][j].estado = mapa[i][j].n_estado;
}

void *run(arg *args)
{
    int i;
    int j;
    int n;
    int k = args->p;
    int f = args->q;
    int nvis = args->n;

    for (n = 0; n < nvis; n++)
    {
        for (i = k; i < f; i++)
        {
            for (j = 0; i < cols; j++)
            {
                pthread_mutex_lock(&mutex);
                gen_next_state(i,j);
                pthread_mutex_unlock(&mutex);
            }
            
        }

        while (!update());

        for (i = k; i < f; i++)
        {
            for (j = 0; i < cols; j++)
            {
                pthread_mutex_lock(&mutex);
                update_state(i,j);
                pthread_mutex_unlock(&mutex);
            }
            
        }

    }

    pthread_exit(NULL);
}

int update()
{
    int f = 1;
    int i, j;

    for (i = 0; i < filas && f; i++)
    {
        for (j = 0; j < cols; j++)
        {
            f = f && flag[i][j] == 1;
        }
        
    }
     return f;
}