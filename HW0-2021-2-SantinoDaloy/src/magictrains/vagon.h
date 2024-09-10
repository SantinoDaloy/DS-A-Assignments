#pragma once
#include "asiento.h"
#include <stdio.h>
#include <stdlib.h>
typedef struct vagon {
  int id;
  int capacidad;
  Asiento** asientos;
} Vagon;

Vagon* vagon_init(int id, int capacidad);
void vagon_destroy(Vagon* vagon);