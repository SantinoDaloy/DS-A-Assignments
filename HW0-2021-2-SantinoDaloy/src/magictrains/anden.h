#pragma once
#include "tren.h"
#include "list.h"
#include <stdio.h>
#include <stdlib.h>
#include "pasajero.h"

typedef struct anden {
  int id;
  int tiene_tren;
  List* ultimo_en_fila_normal;
  List* ultimo_en_fila_premium;
  List* pasajeros_normal;
  List* pasajeros_premium;
  Tren* tren;
} Anden;

Anden* anden_init(int id);
void anden_destroy(Anden* anden);