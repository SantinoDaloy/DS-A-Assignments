#include "list.h"
#include <stdio.h>
#include <stdlib.h>

List* list_init(Pasajero* value)
{
  List* list = malloc(sizeof(List));

  *list = (List) {
    .value = value,
    .next = NULL,
  };
  return list;
}

List* list_append(List* list, Pasajero* value, List* last)
{
  List *new_list = list_init(value);

  last->next = new_list;
  return new_list;
}

void list_print(List* list, int primer_llamado, FILE* output_file)
{
  if (primer_llamado == 1)
  {
    if (list->next)
    {
      list_print(list->next, 0, output_file);
    }
  }
  else if (primer_llamado == 0)
  {
    fprintf(output_file,"%i-%i-%i ", list->value->id, list->value->destino, list->value->categoria);
    if (!list->next)
    {
      return;
    }
    else
    {
      list_print(list->next, 0, output_file);
    }
  }
}
Pasajero* list_at_index(List* list, int index)
{
  List* node = list;
  for (size_t i = 0; i < index; i++)
  {
    node = node->next;
  }

  return node->value;
}
List* lista_en_lugar(List* list, int index)
{
  List* node = list;
  for (size_t i = 0; i < index; i++)
  {
    node = node->next;
  }

  return node;
}
int list_last(List* list, int cuenta)
{
  int contador = cuenta;
  if (!list->next)
  {
    return cuenta;
  }
  else
  {
    contador++;
    list_last(list->next, contador);
  }
}
void list_destroy(List *list)
{
  if (list->next)
  {
    list_destroy(list->next);
  }
  
  pasajero_destroy(list->value);
  free(list);
}

void list_destroy_2(List *list, Pasajero* pasajero)
{
  if (list->next)
  {
    if (list->value != pasajero)
    {
      list_destroy_2(list->next, pasajero);
    }
  }
  if (list->value != pasajero)
  {
    free(list);
  }
  
}
void list_destroy_3(List *list)
{
  if (list->next)
  {
    list_destroy_3(list->next);
  }
  
  free(list);
}
void delete(List* list, int posicion, int cuenta)
{
  if (cuenta==posicion-1)
  {
    List* borrar;
    List* next_next;
    next_next = list->next->next;
    borrar = list->next;
    list->next = next_next;
    free(borrar);
    return;
  }
  else
  {
    int mas_1;
    mas_1 = cuenta;
    mas_1++;
    delete(list->next, posicion, mas_1);
    return;
  }
}