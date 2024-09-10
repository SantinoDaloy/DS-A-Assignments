#include "pasajero.h"
#include <stdio.h>
#include <stdlib.h>

Pasajero* pasajero_init(int id, int estacion, int anden, int destino, int categoria)
{
  Pasajero* pasajero_nuevo = malloc(sizeof(Pasajero));

  *pasajero_nuevo = (Pasajero) {
    .id = id,
    .estacion = estacion,
    .anden = anden,
    .destino = destino,
    .categoria = categoria,
  };
  return pasajero_nuevo;
}

void pasajero_destroy(Pasajero* pasajero)
{
  free(pasajero);
}