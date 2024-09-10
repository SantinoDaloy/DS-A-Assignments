#pragma once
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "tree.h"
#include "../imagelib/image.h"

typedef struct list List;

typedef struct alpha_filter{
    Component_tree_node** nodos;
    int cant_nodos;
} Alpha_filter;

Alpha_filter* filter_init(int cant_pixeles);
void recorrer_nodos(Alpha_filter* filter, Image* marker, Image* image, float alpha);
float costo_agregar(Component_tree_node* nodo, Image* marker, int leaf, Image* image, float alpha);
float costo_no_agregar(Component_tree_node* nodo, Image* marker, int leaf, Image* image, float alpha);