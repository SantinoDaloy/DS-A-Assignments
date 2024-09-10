#include "alpha.h"
#include <stdio.h>
#include <stdlib.h>
#include "tree.h"
#include "../imagelib/image.h"

Alpha_filter* filter_init(int cant_nodos)
{
  Alpha_filter* filter = malloc(sizeof(Alpha_filter));

  *filter = (Alpha_filter) {
    .nodos = calloc(cant_nodos, sizeof(Component_tree_node*)),
    .cant_nodos = cant_nodos,
  };
  return filter;
}

void recorrer_nodos(Alpha_filter* filter, Image* marker, Image* image, float alpha){
    int n=(filter->cant_nodos)-1;
    float costo_asignar=0;
    float costo_no_asignar=0;
    while (n>=0){
        filter->nodos[n]->incluidos_momento->tail = filter->nodos[n]->incluidos_momento;
        if (filter->nodos[n]->cant_hijos == 0){
            filter->nodos[n]->incluidos_momento->tail = filter->nodos[n]->incluidos_momento;
            costo_asignar = costo_agregar(filter->nodos[n], marker, 1, image, alpha);
            costo_no_asignar = costo_no_agregar(filter->nodos[n], marker, 1, image, alpha);
            if (costo_asignar < costo_no_asignar){
                filter->nodos[n]->cost = costo_asignar;
                filter->nodos[n]->incluidos_momento->value = filter->nodos[n];
                filter->nodos[n]->incluidos_momento->tail = filter->nodos[n]->incluidos_momento;
                filter->nodos[n]->incluidos_momento->next = NULL;
            }
            else{
                filter->nodos[n]->cost = costo_no_asignar;
            }
        }
        else{
            filter->nodos[n]->incluidos_momento->tail = filter->nodos[n]->incluidos_momento;
            costo_asignar = costo_agregar(filter->nodos[n], marker, 0, image, alpha);
            costo_no_asignar = costo_no_agregar(filter->nodos[n], marker, 0, image, alpha);
            if (costo_asignar < costo_no_asignar){
                filter->nodos[n]->cost = costo_asignar;
                filter->nodos[n]->incluidos_momento->value = filter->nodos[n];
                filter->nodos[n]->incluidos_momento->tail = filter->nodos[n]->incluidos_momento;
                filter->nodos[n]->incluidos_momento->next = NULL;
            }
            else{
                List* analizar = filter->nodos[n]->hijos;
                filter->nodos[n]->cost = costo_no_asignar;
                if (filter->nodos[n]->cant_hijos == 1){
                    if (filter->nodos[n]->hijos->value != NULL){
                        filter->nodos[n]->incluidos_momento = filter->nodos[n]->hijos->value->incluidos_momento;
                    }
                }
                else{
                    while (analizar->next){
                        filter->nodos[n]->incluidos_momento->tail->next = analizar->value->incluidos_momento;
                        filter->nodos[n]->incluidos_momento->tail = analizar->value->incluidos_momento->tail;
                        analizar = analizar->next;
                    }
                    filter->nodos[n]->incluidos_momento->tail->next = analizar->value->incluidos_momento;
                    filter->nodos[n]->incluidos_momento->tail = analizar->value->incluidos_momento->tail;
                }
            }
            
        }
        n--;
    }
}

float costo_agregar(Component_tree_node* nodo, Image* marker, int leaf, Image* image, float alpha){
    int count = 0;
    float cost = 0;
    for (int p=0; p<nodo->cant_usada;p++){
        if (marker->pixels[nodo->pixels[p]] == 0){
            count++;
        }
    }
    cost = alpha * (count);
    return cost;
}

float costo_no_agregar(Component_tree_node* nodo, Image* marker, int leaf, Image* image, float alpha){

    //REVISO MENOR UMBRAL DE LOS HIJOS
    float cost=0;
    List* analizando = nodo->hijos;
    //REVISO CUANTOS PIXELES PROPIOS PERTENECEN A MARKER
    int count = 0;
    for (int p=0; p<nodo->cant_usada;p++){
        if (marker->pixels[nodo->pixels[p]] != 0 && image->pixels[nodo->pixels[p]] == nodo->umbral){
            count++;
        }
    }
    cost = (1-alpha) * (count);
    if (leaf == 0){
        if (nodo->cant_hijos == 1){
            cost = ((1-alpha) * (count)) + analizando->value->cost;
        }
        else{
            while (analizando->next){
                cost += analizando->value->cost;
                analizando = analizando->next;

            }
            cost += analizando->value->cost;
        }
    }
    return cost;
}