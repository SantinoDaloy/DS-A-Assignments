#include "estacion.h"
#include <stdio.h>
#include <stdlib.h>
Estacion* estacion_init(int id, int cant_andenes)
{
  Estacion* estacion_nuevo = malloc(sizeof(Estacion));

  *estacion_nuevo = (Estacion) {
    .id = id,
    .andenes = calloc(cant_andenes, sizeof(Anden*)),
    .cant_andenes = cant_andenes,
  };
  return estacion_nuevo;
}
void estacion_destroy(Estacion* estacion)
{
  free(estacion->andenes);
  free(estacion);
}
