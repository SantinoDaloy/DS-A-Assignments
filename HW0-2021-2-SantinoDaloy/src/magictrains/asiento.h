#pragma once
#include "pasajero.h"
#include <stdio.h>
#include <stdlib.h>
typedef struct asiento {
  int id;
  int ocupado;
  Pasajero* pasajero;
} Asiento;

Asiento* asiento_init(int id);
void asiento_destroy(Asiento* asiento);