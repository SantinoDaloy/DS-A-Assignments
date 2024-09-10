#pragma once
#include "anden.h"
#include <stdio.h>
#include <stdlib.h>

typedef struct estacion {
  int id;
  Anden** andenes;
  int cant_andenes;
} Estacion;

Estacion* estacion_init(int id, int cant_andenes);
void estacion_destroy(Estacion* estacion);