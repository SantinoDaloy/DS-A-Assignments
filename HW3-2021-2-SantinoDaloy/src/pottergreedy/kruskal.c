#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "kruskal.h"

MST* make_mst(int V, int E){
    MST* mst = malloc(1*sizeof(MST));
    mst->V = V;
    mst->E = E;
    mst->aristas = (Arista*)malloc(E*sizeof(Arista));
    return mst;
}

int manhattan_dist(Punto punto_a, Punto punto_b){
    return abs(punto_a.x - punto_b.x) + abs(punto_a.y - punto_b.y);
}

int kruskal(MST* mst, FILE* output_stream){
    int costo_minimo = 0;
    Set* sets = (Set*)malloc(mst->V * sizeof(Set));

    for(int v=0; v<mst->V; v++){
        sets[v].rango = 0;
        sets[v].r = v;
    }

    int count = 0;

    qsort(mst->aristas, mst->E, sizeof(Arista), lessthan);
    for(int i=0; i<mst->E; i++){

        Arista edge = mst->aristas[i];
        Punto* punto_a = edge.punto_a;
        Punto* punto_b = edge.punto_b;
        //printf("%i\n", edge.costo);
        int representante_a = get_representante(sets, punto_a->id);
        // printf("representante del nodo %d: %d\n", punto_a->id, representante_a);
        int representante_b = get_representante(sets, punto_b->id);

        if(representante_a != representante_b){
                count++;
                costo_minimo+= edge.costo;
                fprintf(output_stream, "%d %d %d %d\n", mst->aristas[i].punto_a->y, mst->aristas[i].punto_a->x, mst->aristas[i].punto_b->y, mst->aristas[i].punto_b->x);
                make_union(sets, representante_a, representante_b);}
                

        if (count >= mst->V-1){
            break;
        }
    }
    free(sets);
    return costo_minimo;
}

void create_aristas(MST* mst, Punto* puntos){
    int count = 0;
    for(int i=0; i<mst->V; i++){
        for(int j=i+1; j<mst->V; j++){
            Arista arista = mst->aristas[count];
            arista.punto_a = &puntos[i];
            arista.punto_b = &puntos[j];
            arista.costo = manhattan_dist(puntos[i], puntos[j]);
            mst->aristas[count] = arista;
            count++;
        }
    }
    mst->E = count;
}


int lessthan(const void* a, const void* b)
{
    Arista* a1 = (Arista*)a;
    Arista* b1 = (Arista*)b;
    return a1->costo > b1->costo;
}