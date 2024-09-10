#pragma once
#include "vagon.h"
#include <stdio.h>
#include <stdlib.h>
typedef struct tren {
  int id;
  Vagon** vagones;
  int cant_vagones;
} Tren;
Tren* tren_init(int id, int cant_vagones);
void estado_tren(Tren* tren, FILE* output_file);
void tren_destroy(Tren* tren);