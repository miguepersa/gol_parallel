#include "golh.h"

celula mapa[MAX_FIL][MAX_COL];
int flag[MAX_FIL][MAX_COL];

/* Bloqueador mutex */
pthread_mutex_t mutex = PTHREAD_MUTEX_INITIALIZER;

/* Numero de filas y columnas del mapa */
int filas, cols;


void golh(celula m[][MAX_COL], int f[][MAX_COL], int nfilas, int ncols, int nhilos, int ngens, int nvis)
{
    /* Arreglo de identificadores de hilos */
    pthread_t hilos[MAX_ARR];

    /* Variable de iteracion */
    int i, j;

    /* Variable que se utiliza para asignar filas a hilos */
    int k = nfilas / nhilos;
    
    /* Variables que definen los limites de los hilos */
    int p = 0;
    int q = 0;

    /* Variable donde almacenan los argumentos que recibe el runner */
    arg args[MAX_ARR];

    /* Numero de filas y columnas del mapa */
    filas = nfilas;
    cols = ncols;

    for (i = 0; i < nfilas; i++)
    {
        for (j = 0; j < ncols; j++)
        {
            mapa[i][j].estado = m[i][j].estado;
            mapa[i][j].n_estado = m[i][j].n_estado;
        }
    }

    printf("Generacion inicial:\n");
    print_gen(mapa, filas, cols);

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
            args[i].n = nvis;

            if (pthread_create(&hilos[i], NULL, run, (void*) &args[i]) != 0)
            {
                perror("Error al crear hilo\n");
            }

            p = p + k;
        }

        for (i = 0; i < nhilos; i++)
        {
            if (pthread_join(hilos[i], NULL) != 0)
            {
                perror("Error en el hilo %d\n");
            }
        }
        
        printf("Generacion %d\n", j+nvis);
        print_gen(mapa, filas, cols);
    }

    if (ngens%nvis != 0)
    {
        print_gen(mapa, filas, cols);
    }
    
    return;
}

void gen_next_state(int i, int j)
{
    /* Numero de vecinos de la celula en la posicion i,j */
    int vecinos = 0;
    
    vecinos = count_vecinos(mapa, i, j, filas, cols);

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

void *run(void *args)
{
    int i;
    int j;
    int n;
    arg a = *(arg*)args;
    int k = a.p;
    int f = a.q;
    int nvis = a.n;

    for (n = 0; n < nvis; n++)
    {
        for (i = k; i < f; i++)
        {
            for (j = 0; i < cols; j++)
            {
                pthread_mutex_lock(&mutex);
                gen_next_state(i,j);
                flag[i][j] = 1;
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
                flag[i][j] = 0;
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