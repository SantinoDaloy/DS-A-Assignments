#include "vagon.h"
#include <stdio.h>
#include <stdlib.h>

Vagon* vagon_init(int id, int capacidad)
{
  Vagon* vagon_nuevo = malloc(sizeof(Vagon));

  *vagon_nuevo = (Vagon) {
    .id = id, 
    .capacidad = capacidad,
    .asientos = calloc(capacidad, sizeof(Asiento*)),
  };
  return vagon_nuevo;
}

void vagon_destroy(Vagon* vagon)
{
  free(vagon->asientos);
  free(vagon);
}