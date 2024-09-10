#include "tree.h"
#include <stdio.h>
#include <stdlib.h>

Component_tree_node* tree_node_init(int umbral, int cant_maxima)
{
  Component_tree_node* node = malloc(sizeof(Component_tree_node));

  *node = (Component_tree_node) {
    .umbral = umbral,
    .pixels = calloc(cant_maxima, sizeof(int)),
    .hijos = list_init(NULL),
    .cant_usada = 0,
    .cero_usado = 0,
    .cant_hijos = 0,
    .cost = 0,
    .incluidos_momento = queue_init(NULL),
  };
  return node;
}

int cant_max_vecindario(int umbral, Component_tree_node* nodo, Image* image){
    int count;
    count = 0;
    for (int a=0; a < nodo->cant_usada;a++){
        if (image->pixels[nodo->pixels[a]] >= umbral){
            count++;
        }
    }
    return count;
}

void create_childs(Component_tree_node* nodo, Image* image, Queue* queue){
    //printf("LLAMA CREATE CHILDS de %i\n", nodo->cant_usada);
    if (nodo->umbral >= 127){
        return;
    }
    int cant_max, asignados, sig_umbral, actual;
    actual=0;
    asignados = 0;
    //SIG UMBRAL
    int count = 127;
    int cuen = 0;
    for (int a=0; a < nodo->cant_usada;a++){
        if (image->pixels[nodo->pixels[a]] < count){
            if (image->pixels[nodo->pixels[a]] > nodo->umbral){
                count = image->pixels[nodo->pixels[a]];
            }
            if (image->pixels[nodo->pixels[a]] == nodo->umbral){
                cuen++;
            }
        }
    }
    sig_umbral = count;

    //CANT MAX
    cant_max = nodo->cant_usada - cuen;
    //cant_max = cant_max_vecindario(sig_umbral, nodo, image);
    int hijos_creados = 0;
    if (cant_max == 0){
        return;
    }
    if (cant_max == nodo->cant_usada){
        nodo->umbral = sig_umbral;
    }
    int* usados_def;
    usados_def = calloc(image->pixel_count, sizeof(int));
    while (asignados < cant_max){ //OJO ACA
        Component_tree_node* nuevo_nodo;
        nuevo_nodo = tree_node_init(sig_umbral, cant_max);
        actual = asign_neighborhood(nuevo_nodo, nodo, cant_max, image, asignados, usados_def);
        if ((actual - asignados) == 0){
            break;
        }
        // APPEND NUEVO NODO A LA LISTA DE HIJOS
        if (hijos_creados == 0){
            nodo->hijos->value = nuevo_nodo;
            nodo->hijos->tail = nodo->hijos;
            nuevo_nodo->cant_usada = actual - asignados;
            hijos_creados++;
        }
        else{
            list_append(nodo->hijos, nuevo_nodo);
            nuevo_nodo->cant_usada = actual - asignados;
            hijos_creados++;
        }
        if (nuevo_nodo->umbral <= 127){
            //AGREGAR A LA COLA
            int count = 127;
            for (int a=0; a < nuevo_nodo->cant_usada;a++){
                if (image->pixels[nuevo_nodo->pixels[a]] < count){
                    if (image->pixels[nuevo_nodo->pixels[a]] >= nuevo_nodo->umbral){
                        count = image->pixels[nuevo_nodo->pixels[a]];
                    }
                }
            }
            nuevo_nodo->umbral = count;
            nuevo_nodo->pixels = realloc(nuevo_nodo->pixels, nuevo_nodo->cant_usada*sizeof(int));
            queue_append(queue, nuevo_nodo);
        }
        asignados = actual;
    }
    nodo->cant_hijos = hijos_creados;
    free(usados_def);
    //LLAMAR FUNCION PARA TODOS LOS HIJOS CREADOS
}

int asign_neighborhood(Component_tree_node* nodo, Component_tree_node* padre, int cant_max, Image* image, int asignados, int* usados_def){
    // revisar todos lospixeles, si encuentro uno que sirve, 
    // reviso si el adyacente a los que ya estan en el array, si lo es, lo agrego, y lo pongo el los ocupados del nodo padre
    // luego con la cuenta de todos los agregados, reviso si hay que agregar más, si es así
    int asign, cuenta_actual, previo;
    previo = asignados;
    cuenta_actual = 1;
    asign = asignados;
    QueueVecinos* queue_vecinos;
    QueueVecinos* primero;
    queue_vecinos = queue_vecinos_init(0);
    queue_vecinos->tail = queue_vecinos;
    primero = queue_vecinos;
    if (nodo->umbral >= 128){
        return asignados;
    }
    for (int p=0;p<=padre->cant_usada;p++){
        if (p != 0 && padre->pixels[p] == 0){
            break;
        }
        //reviso si cumple con umbral geq
        if (image->pixels[padre->pixels[p]] >= nodo->umbral){
            //primero reviso si está usado
            int usado = 0;
            if (usados_def[padre->pixels[p]] == 1){
                usado++;
            }
            if (usado==0){
                // si no hay nada asignado en ese array, asignarlo si o si
                if (padre->pixels[p] == 0){
                    padre->cero_usado = 1;
                }
                nodo->pixels[0] = padre->pixels[p];
                usados_def[padre->pixels[p]] = 1;
                //printf("PRIMERO %i, \n", padre->pixels[p]);
                asign++;
                queue_vecinos->value = padre->pixels[p];
                asign = revisar_vecinos(padre->pixels, queue_vecinos->value, asign, nodo, image, padre, queue_vecinos, cant_max, cuenta_actual, usados_def);
                cuenta_actual = asign - previo;
                while (queue_vecinos->next){
                    queue_vecinos->next->tail = queue_vecinos->tail;
                    queue_vecinos = queue_vecinos->next;
                    asign = revisar_vecinos(padre->pixels, queue_vecinos->value, asign, nodo, image, padre, queue_vecinos, cant_max, cuenta_actual, usados_def);
                    cuenta_actual = asign - previo;
                }
                //printf("heree%i\n",primero->next->value);
                queue_vecinos_destroy(primero);
                //asign = revisar_vecinos(padre->pixels, queue_vecinos->value, asign, nodo, image, padre, queue_vecinos, cant_max, cuenta_actual);
                //cuenta_actual = asign - previo;
                break;
            }
        }
    }
    //printf("asignados %i\n", asign);
    return asign;
}
int revisar_vecinos(int* pixels, int pixel, int asignados, Component_tree_node* nodo, Image* image, Component_tree_node* padre, QueueVecinos* queue, int cant_max, int cuenta_actual, int* usados_def){
    int arr, abaj, der, izq, cuent;
    cuent = cuenta_actual;
    arr = 0;
    abaj = 0;
    der = 0;
    izq = 0;
    int asig = asignados;
    //for (int o=0; o <= cant_max;o++){
    //if (o != 0 && padre->usados[o] == 0){
    //}
    if (pixel-image->width == 0){
        arr++;
    }
    if (pixel-1 == 0){
        izq++;
    }
    if ((pixel + 1)%image->width != 0 && (pixel+1)<image->pixel_count && (pixel + 1) != 0){
        if (usados_def[pixel+1] == 1){
            der++;
        }
    }
    if ((pixel)%image->width != 0 && (pixel-1)>=0 && (pixel-1)!=0){
        if (usados_def[pixel-1] == 1){
            izq++;
        }
    }
    if ((pixel-image->width) > 0){
        if (usados_def[pixel-image->width] == 1){
            arr++;
        }
    }
    if ((pixel+image->width) < image->pixel_count){
        if (usados_def[pixel+image->width] == 1){
            abaj++;
        }
    }
    if (izq>0 && der > 0 && arr > 0 && abaj > 0){
        return asig;
    }
    //}
    if ((pixel + 1)%image->width != 0 && (pixel+1)<image->pixel_count){
        if (image->pixels[pixel + 1] >= nodo->umbral && der==0){
            nodo->pixels[cuent] = pixel+1;
            usados_def[pixel+1] = 1;
            //printf("%i, \n", pixel+1);
            cuent++; 
            asig++;
            queue_vecinos_append(queue, pixel+1);
        }
    }
    if ((pixel)%image->width != 0 && (pixel-1)>=0){
        if (image->pixels[pixel-1] >= nodo->umbral && izq==0){
            nodo->pixels[cuent] = pixel-1;
            usados_def[pixel-1] = 1;
            //printf("%i, \n", pixel-1);
            asig++;
            cuent++;
            queue_vecinos_append(queue, pixel-1);
        }
    }
    if ((pixel-image->width) >= 0){
        if (image->pixels[pixel - image->width] >= nodo->umbral && arr==0){
            nodo->pixels[cuent] = pixel-(image->width);
            usados_def[pixel-(image->width)] = 1;
            asig++;
            cuent++;
            //printf("%i, \n", pixel-image->width);
            queue_vecinos_append(queue, pixel-(image->width));
        }
    }
    if ((pixel+image->width) < image->pixel_count){
        if (image->pixels[pixel + image->width] >= nodo->umbral && abaj==0){
            nodo->pixels[cuent] = pixel+(image->width);
            usados_def[pixel+(image->width)] = 1;
            asig++;
            cuent++;
            //printf("%i, \n", pixel+image->width);
            queue_vecinos_append(queue, pixel+(image->width));
        }
    }
    return asig;
}

// ######### LIST ############# //

List* list_init(Component_tree_node* value)
{
  List* list = malloc(sizeof(List));

  *list = (List) {
    .value = malloc(sizeof(Component_tree_node*)),
    .next = NULL,
    .tail = NULL,
  };
  return list;
}

void list_append(List* list, Component_tree_node* value)
{
  //List *last = list;
  //while (last->next) {
  //  last = last->next;
  //}
  List *new_list = list_init(value);
  list->tail->next = new_list;
  list->tail->next->value = value;
  list->tail = new_list;
}

// ######### Queue ############# //

Queue* queue_init(Component_tree_node* value)
{
  Queue* queue = malloc(sizeof(Queue));

  *queue = (Queue) {
    .value = NULL,
    .next = NULL,
    .tail = NULL,
  };
  return queue;
}

void queue_append(Queue* queue, Component_tree_node* value)
{
  Queue* new_queue = malloc(sizeof(Queue));
  new_queue = queue_init(value);
  new_queue->value = value;
  queue->tail->next = new_queue;
  queue->tail = new_queue;
}

void queue_destroy(Queue* queue){
    //printf("entra\n");
    Queue* analizar;
    Queue* aux;
    analizar = queue;
    aux = queue;
    while (analizar->next){
        aux = analizar;
        analizar = analizar->next;
        free(aux);
    }
    free(analizar);
    //printf("sale\n");
}
// ######### Queue ############# //

QueueVecinos* queue_vecinos_init(int value)
{
  QueueVecinos* queue = malloc(sizeof(QueueVecinos));

  *queue = (QueueVecinos) {
    .value = 0,
    .next = NULL,
    .tail = NULL,
  };
  return queue;
}

void queue_vecinos_append(QueueVecinos* queue, int value)
{
  QueueVecinos* new_queue = malloc(sizeof(QueueVecinos));
  new_queue = queue_vecinos_init(value);
  new_queue->value = value;
  queue->tail->next = new_queue;
  queue->tail = new_queue;
}

void queue_vecinos_destroy(QueueVecinos* queue){
    //printf("entra\n");
    QueueVecinos* analizar;
    QueueVecinos* aux;
    analizar = queue;
    aux = queue;
    while (analizar->next){
        aux = analizar;
        analizar = analizar->next;
        free(aux);
    }
    //printf("sale\n");
    free(analizar);

}