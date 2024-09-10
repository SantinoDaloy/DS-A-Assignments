#pragma once
#include <stdio.h>
#include <stdlib.h>
typedef struct pasajero {
  int id;
  int estacion;
  int anden;
  int destino;
  int categoria;
} Pasajero;

Pasajero* pasajero_init(int id, int estacion, int anden, int destino, int categoria);
void pasajero_destroy(Pasajero* pasajero);