#include "tren.h"
#include "vagon.h"
#include <stdio.h>
#include <stdlib.h>

Tren* tren_init(int id, int cant_vagones)
{
  Tren* tren_nuevo = malloc(sizeof(Tren));

  *tren_nuevo = (Tren) {
    .id = id,
    .vagones = calloc(cant_vagones, sizeof(Vagon*)),
    .cant_vagones = cant_vagones,
  };
  return tren_nuevo;
}
void estado_tren(Tren* tren, FILE* output_file)
{
  int cantidad_vagones;
  cantidad_vagones = tren->cant_vagones;
  fprintf(output_file, "T%i", tren->id);
  fprintf(output_file, " |");
  for (int v=0; v < cantidad_vagones;v++)
  {
    for (int a=0; a<tren->vagones[v]->capacidad;a++)
    {
      if (tren->vagones[v]->asientos[a]->ocupado == 0)
      {
        fprintf(output_file, " X");
      }
      else
      {
        fprintf(output_file, " %i-%i", tren->vagones[v]->asientos[a]->pasajero->id, tren->vagones[v]->asientos[a]->pasajero->destino);
      }
    }
    fprintf(output_file, " |");
  }
  fprintf(output_file, "\n");
}

void tren_destroy(Tren* tren)
{
  free(tren->vagones);
  free(tren);
}