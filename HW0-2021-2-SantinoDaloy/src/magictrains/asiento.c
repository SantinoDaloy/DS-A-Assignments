#include "asiento.h"
#include <stdio.h>
#include <stdlib.h>

Asiento* asiento_init(int id)
{
  Asiento* asiento_nuevo = malloc(sizeof(Asiento));

  *asiento_nuevo = (Asiento) {
    .id = id, 
    .pasajero = NULL,
    .ocupado = 0,
  };
  return asiento_nuevo;
}

void asiento_destroy(Asiento* asiento)
{
  free(asiento);
}