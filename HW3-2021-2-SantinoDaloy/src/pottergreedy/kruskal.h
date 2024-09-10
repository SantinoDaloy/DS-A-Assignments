#pragma once
#include <stdbool.h>
#include <stdlib.h>
#include <math.h>
#include <memory.h>
#include "set.h"

typedef struct mst{
    int V;
    int E;
    Arista* aristas;
    Punto* aux_punto;
} MST;




MST* make_mst(int V, int E);

int kruskal(MST* mst, FILE* output_stream);

void create_aristas(MST* mst, Punto* puntos);
int lessthan(const void* a, const void* b);

int manhattan_dist(Punto punto_a, Punto punto_b);