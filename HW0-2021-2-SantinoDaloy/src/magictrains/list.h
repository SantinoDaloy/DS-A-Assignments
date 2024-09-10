#pragma once
#include <stdio.h>
#include <stdlib.h>
#include "pasajero.h"

typedef struct list {
  Pasajero* value;
  struct list* next;
} List;

List* list_init(Pasajero* value);
List* list_append(List* list, Pasajero* value, List* last);
void list_print(List* list,  int primer_llamado, FILE* output_file);
Pasajero* list_at_index(List* list, int index);
int list_last(List* list, int cuenta);
void list_destroy(List *list);
List* lista_en_lugar(List* list, int index);
void list_destroy_2(List* list, Pasajero* pasajero);
void list_destroy_3(List *list);