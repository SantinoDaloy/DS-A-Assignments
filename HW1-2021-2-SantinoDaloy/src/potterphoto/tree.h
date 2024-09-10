#pragma once
#include "../imagelib/image.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct list List;

typedef struct component_tree_node{
   int umbral;
	int* pixels;
   List* hijos;
   int cant_usada;
   int cero_usado;
   int cant_hijos;
   float cost;
   struct queue* incluidos_momento;
} Component_tree_node;


typedef struct list {
  Component_tree_node* value;
  struct list* next;
  struct list* tail;
} List;

typedef struct queue {
   Component_tree_node* value;
   struct queue* next;
   struct queue* tail;
} Queue;

typedef struct queue_vecinos {
   int value;
   struct queue_vecinos* next;
   struct queue_vecinos* tail;
} QueueVecinos;



List* list_init(Component_tree_node* value);
void list_append(List* list, Component_tree_node* value);
Queue* queue_init(Component_tree_node* value);
void queue_append(Queue* queue, Component_tree_node* value);
Component_tree_node* tree_node_init(int umbral, int cant_maxima);
int cant_max_vecindario(int umbral, Component_tree_node* nodo, Image* image);
void create_childs(Component_tree_node* nodo, Image* image, Queue* queue);
int asign_neighborhood(Component_tree_node* nodo, Component_tree_node* padre, int cant_max, Image* image, int asignados, int* usados_def);
int check_adyacente(int* pixeles, int pixel, int asignados, Image* image);
int revisar_vecinos(int* pixels, int pixel, int asignados, Component_tree_node* nodo, Image* image, Component_tree_node* padre, QueueVecinos* queue, int cant_max, int cuenta_actual, int* usados_def);
void queue_vecinos_append(QueueVecinos* queue, int value);
QueueVecinos* queue_vecinos_init(int value);
void queue_vecinos_destroy(QueueVecinos* queue);
void queue_destroy(Queue* queue);

