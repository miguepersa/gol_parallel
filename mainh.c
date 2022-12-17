#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "golh.h"

int main(int argc, char const *argv[])
{
    /* Variables de entrada */
    int n_hilos;
    int n_gen;
    int n_vis;
    int nfilas;
    int ncols;
    int i,j;
    char n_archivo[128];
    
    /*if (argc != 4){
        printf("Error de argumentos\n");
        return 0;
    }*/

    /* Se reciben los argumentos en el programa */
    n_hilos = atoi(argv[1]);
    n_gen = atoi(argv[2]);
    n_vis = atoi(argv[3]);
    strcpy(n_archivo, argv[4]);

    /* Se procesa el archivo de entrada y se almacenan sus datos */
    procesar_archivo(mapa, n_archivo, &nfilas, &ncols);

    golh(nfilas, ncols, n_hilos, n_gen, n_vis);

    return 0;
}
