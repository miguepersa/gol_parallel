#include <stdio.h>
#include <string.h>

#include "golh.h"

int main(int argc, char const *argv[])
{
    /* Variables de entrada */
    int n_hilos;
    int n_gen;
    int n_vis;
    int nfilas;
    int ncols;
    char n_archivo[128];

    /* Mapa del juego */
    celula mapa[MAX_FIL][MAX_COL];

    /* Arreglo utilizado como flag para actualizar estados */
    int flag[MAX_FIL][MAX_COL];

    if (argc != 5){
        printf("Error de argumentos\n");
        return 0;
    }

    /* Se reciben los argumentos en el programa */
    n_hilos = atoi(argv[1]);
    n_gen = atoi(argv[2]);
    n_vis = atoi(argv[3]);
    strcpy(n_archivo, argv[4]);

    /* Se procesa el archivo de entrada y se almacenan sus datos */
    procesar_archivo(mapa, n_archivo, &nfilas, &ncols);

    golh(mapa, flag, nfilas, ncols, n_hilos, n_gen, n_vis);

    return 0;
}
