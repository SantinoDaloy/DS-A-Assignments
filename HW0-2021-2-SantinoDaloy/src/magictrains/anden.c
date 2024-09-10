#include "anden.h"
#include <stdio.h>
#include <stdlib.h>
#include "list.h"
#include "pasajero.h"

Anden* anden_init(int id)
{
  Anden* anden_nuevo = malloc(sizeof(Anden));

  List* ini_normal = list_init(NULL);
  List* ini_premium = list_init(NULL);
  *anden_nuevo = (Anden) {
    .id = id,
    .tiene_tren = 0,
    .ultimo_en_fila_normal = ini_normal,
    .ultimo_en_fila_premium = ini_premium,
    .pasajeros_normal = ini_normal,
    .pasajeros_premium = ini_premium,
    .tren = NULL,
  };
  return anden_nuevo;
}

void anden_destroy(Anden* anden)
{
  list_destroy(anden->pasajeros_normal);
  list_destroy(anden->pasajeros_premium);
  free(anden);
}