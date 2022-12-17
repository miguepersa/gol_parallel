#include "funciones.h"

void procesar_archivo(celula m[][MAX_COL], char *archivo, int *nfilas, int *ncols)
{
    int i; /* Variable de iteracion */
    int j; /* Variable de iteracion */
    char buffer[2048]; /* String buffer */
    char aux[2];    /* String buffer auxiliar */
    FILE *f = fopen(archivo, "r"); /* Apertura del archivo */

    /* Lectura de la primera linea del archivo de entrada */
    if (fgets(buffer, 2048, f) != NULL)
    {
        /* Se almacena el numero de filas y columnas en sus variables */
        sscanf(buffer, "%d %d\n", nfilas, ncols);
    }
    
    j = 0;
    aux[1] = '\0';

    /* Lectura y almacenamiento de los datos del archivo */
    while (fgets(buffer, *ncols+1, f) != NULL && j < *nfilas)
    {
        for (i = 0; i < *ncols; i++)
        {
            aux[0] = buffer[i]; 
            sscanf(aux, "%d", &m[j][i].estado);
            m[j][i].n_estado = -1;
        }
        j++;
    }
    
    fclose(f);
}

void print_gen(celula m[][MAX_COL], int nfilas, int ncols)
{
    /* Variables de iteracion */
    int i = 0;
    int j = 0;

    for (i = 0; i < nfilas; i++)
    {
        for (j = 0; j < ncols; j++)
        {
            printf("%d", m[i][j].estado);
        }
        printf("\n");
    }
    
}

int count_vecinos(celula mapa[][MAX_COL], int i, int j, int filas, int cols)
{
    int vecinos = 0;

    if (i - 1 < 0 && j - 1 < 0) /* Esquina superior izquierda */
    {
        vecinos = vecinos + mapa[i][j+1].estado == 1 + mapa[i+1][j+1].estado == 1 + mapa[i+1][j].estado == 1;
    } else if (i - 1 < 0 && j + 1 >= cols) /* Esquina superior derecha */
    {
        vecinos = vecinos + mapa[i][j-1].estado == 1 + mapa[i+1][j-1].estado == 1 + mapa[i+1][j].estado == 1;
    } else if (i + 1 >= filas && j - 1 < 0) /* Esquina inferior izquierda */
    {
        vecinos = vecinos + mapa[i][j+1].estado == 1 + mapa[i-1][j+1].estado == 1 + mapa[i-1][j].estado == 1;
    } else if (i + 1 >= filas && j + 1 >= cols) /* Esquina inferior derecha */
    {
        vecinos = vecinos + mapa[i-1][j].estado == 1 + mapa[i-1][j-1].estado == 1 + mapa[i][j-1].estado == 1;
    } else if (i - 1 < 0 && j - 1 >= 0 && j + 1 < cols) /* Borde superior */
    {
        vecinos = vecinos + mapa[i][j+1].estado == 1 + mapa[i+1][j+1].estado == 1 + mapa[i+1][j].estado == 1 + mapa[i+1][j-1].estado == 1 + mapa[i][j-1].estado == 1;
    } else if (i - 1 >= 0 && j - 1 < 0 && i + 1 < filas) /* Borde izquierdo */
    {
        vecinos = vecinos + mapa[i][j+1].estado == 1 + mapa[i+1][j+1].estado == 1 + mapa[i+1][j].estado == 1 + mapa[i-1][j].estado == 1 + mapa[i-1][j+1].estado == 1;
    }  else if (i - 1 >= 0 && j + 1 >= cols && i + 1 < filas) /* Borde derecho */
    {
        vecinos = vecinos + mapa[i+1][j].estado == 1 + mapa[i+1][j-1].estado == 1 + mapa[i][j-1].estado == 1 + mapa[i-1][j-1].estado == 1 + mapa[i-1][j].estado == 1;
    } else if (i + 1 >= filas && j + 1 <= cols && j - 1 >= 0) /* Borde inferior */
    {
        vecinos = vecinos + mapa[i][j-1].estado == 1 + mapa[i-1][j-1].estado == 1 + mapa[i-1][j].estado == 1 + mapa[i-1][j+1].estado == 1 + mapa[i][j+1].estado == 1;
    } else /* Centro del mapa */
    {
        vecinos = vecinos + mapa[i][j-1].estado == 1 + mapa[i-1][j-1].estado == 1 + mapa[i-1][j].estado == 1 + mapa[i-1][j+1].estado == 1 + mapa[i][j+1].estado == 1 + mapa[i+1][j+1].estado == 1 + mapa[i+1][j].estado == 1 + mapa[i+1][j-1].estado == 1;
    }

    return vecinos;
}