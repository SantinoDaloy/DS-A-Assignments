#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "kruskal.h"
#include "set.h"


int main(int argc, char** argv)
{
    char* option;
    // Revisamos los argumentos
    if(argc < 4) {
        printf("Modo de uso: %s <A/B> <input.txt> <output.txt>\n", argv[0]);
        return 1;
    }

    // Abrimos el archivo de input
    FILE* input_stream = fopen(argv[2], "r");

    // Abrimos el archivo de output
    FILE* output_stream = fopen(argv[3], "w");

    // Si alguno de los dos archivos no se pudo abrir
    if(!input_stream)
    {
        printf("El archivo %s no existe o no se puede leer\n", argv[2]);
        return 2;
    }
    if(!output_stream)
    {
        printf("El archivo %s no se pudo crear o no se puede escribir\n", argv[2]);
        printf("Recuerda que \"fopen\" no puede crear carpetas\n");
        fclose(input_stream);
        return 3;
    }



    option = argv[1];
    if(strcmp(option, "A") == 0){
        // Parte 1 de la Tarea

         /* Lectura de input */

        int size; // Dimensiones de la grid
        int N; // N° de nodos
        int MST_cost; // Costo mínimo base
        
        fscanf(input_stream, "%d %d %d", &size, &N, &MST_cost);

        int row;
        int col;

        Punto* puntos =  (Punto*)calloc(N, sizeof(Punto));

        // Obtenemos cada uno de los puntos
        for(int i = 0; i < N; i++)
        {
            fscanf(input_stream, "%d %d", &row, &col);
            puntos[i].id = i;
            puntos[i].x = col;
            puntos[i].y = row;
        }

        int edges_count = N*(N-1)/2;
        MST* mst = make_mst(N, edges_count);

        create_aristas(mst, puntos);
        fprintf(output_stream, "%i\n", (mst->V));
        int resultado = kruskal(mst, output_stream);

        free(puntos);
        free(mst->aristas);
        free(mst);
        fclose(input_stream);
        return 0;

    }

    else if(strcmp(option, "B") == 0){
        // Parte 2 de la Tarea
        int N; // numero de nodos
        
        fscanf(input_stream, "%d", &N);

        int pos;
        char col;

        // Obtenemos cada uno de los puntos
        for(int i = 0; i < N; i++)
        {
            fscanf(input_stream, "%d %c", &pos, &col);
            printf("Posicion: %d, Color: %c\n", pos, col);
        }
    }

    // Cerrar archivo de input
    fclose(input_stream);

    // Cerrar archivo de output
    fclose(output_stream);

    return 0;
}