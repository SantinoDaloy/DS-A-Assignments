#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "estacion.h"
#include "anden.h"
#include "asiento.h"
#include "pasajero.h"
#include "tren.h"
#include "vagon.h"
#include "list.h"

/* Retorna true si ambos strings son iguales */
bool string_equals(char *string1, char *string2)
{
  return !strcmp(string1, string2);
}

/* Revisa que los parametros del programa sean válidos */
bool check_arguments(int argc, char **argv)
{
  if (argc != 3)
  {
    printf("Modo de uso: %s INPUT OUTPUT\n", argv[0]);
    printf("Donde:\n");
    printf("\tINPUT es la ruta del archivo de input\n");
    printf("\tOUTPUT es la ruta del archivo de output\n");
    return false;
  }

  return true;
}

int main(int argc, char **argv)
{
  /* Si los parámetros del programa son inválidos */
  if (!check_arguments(argc, argv))
  {
    /* Salimos del programa indicando que no terminó correctamente */
    return 1;
  }

  /* Abrimos el archivo de input */
  FILE *input_file = fopen(argv[1], "r");

  /* Abrimos el archivo de output */
  FILE *output_file = fopen(argv[2], "w");


  /* Leemos la cantidad de estaciones y andenes por estación*/
  int N_STATIONS;
  fscanf(input_file, "%d", &N_STATIONS);

  int N_PLATFORMS;
  Estacion** estaciones = calloc(N_STATIONS, sizeof(Estacion*));
  for (int s = 0; s < N_STATIONS; s++)
  {
    fscanf(input_file, "%d", &N_PLATFORMS);
    /*  [Por implementar] Creamos los andenes con la cantidad de andenes dada */
    estaciones[s] = estacion_init(s, N_PLATFORMS);
    for (int j = 0; j < N_PLATFORMS; j++)
    {
      estaciones[s]->andenes[j] = anden_init(j);
    }
  }

  /* String para guardar la instrucción actual*/
  char command[32];

  /* Leemos la primera instrucción */
  fscanf(input_file, "%s", command);
  /* Mientras la instrucción sea distinta a END */
  int contador_trenes;
  Tren* ultimo_que_salio;
  int contador_vagones;
  int contador_pasajeros;
  contador_pasajeros = 0;
  contador_trenes = 0;
  Tren* separar_1;
  Tren* separar_2;
  int vagones_separar_1;
  int vagones_separar_2;
  int cuenta_separar_1;
  int cuenta_separar_2;
  int listo;
  List* nuevo_primero_fila;
  List* nueva_lista;
  while(!string_equals(command, "END"))
  {
    /* [Por implementar] A continuación debes implementar cada evento */
    if (string_equals(command, "NUEVO-TREN"))
    {
      int station_id, platform_id, length, seats;
      fscanf(input_file, "%d %d %d", &station_id, &platform_id, &length);
      estaciones[station_id]->andenes[platform_id]->tren = tren_init(contador_trenes, length);
      estaciones[station_id]->andenes[platform_id]->tiene_tren = 1;
      contador_trenes++;
      contador_vagones = 0;
      for (int l = 0; l < length; l++)
      {
        fscanf(input_file, "%d", &seats);
        estaciones[station_id]->andenes[platform_id]->tren->vagones[l] = vagon_init(contador_vagones, seats);
        contador_vagones++;
        int contador_asientos;
        contador_asientos = 0;
        for (int a = 0;a<seats;a++)
        {
          estaciones[station_id]->andenes[platform_id]->tren->vagones[l]->asientos[a] = asiento_init(contador_asientos);
          contador_asientos++;
        }
      }
      int largo_fila_premium;
      int sente_a_todos;
      sente_a_todos = 1;
      largo_fila_premium = list_last(estaciones[station_id]->andenes[platform_id]->pasajeros_premium, 0);
      for (int p=1;p<=largo_fila_premium;p++)
      {
        listo = 0;
        for (int v = 0; v < estaciones[station_id]->andenes[platform_id]->tren->cant_vagones;v++)
        {
          for (int a = 0; a < estaciones[station_id]->andenes[platform_id]->tren->vagones[v]->capacidad;a++)
          {
            if (estaciones[station_id]->andenes[platform_id]->tren->vagones[v]->asientos[a]->ocupado == 0)
            {
              estaciones[station_id]->andenes[platform_id]->tren->vagones[v]->asientos[a]->pasajero = list_at_index(estaciones[station_id]->andenes[platform_id]->pasajeros_premium, p);
              listo++;
              estaciones[station_id]->andenes[platform_id]->tren->vagones[v]->asientos[a]->ocupado = 1;
              break;
            }
          }
          if (listo == 1)
          {
            break;
          }
        }
        if (listo==0)
        {
          sente_a_todos = 0;
          nuevo_primero_fila = lista_en_lugar(estaciones[station_id]->andenes[platform_id]->pasajeros_premium, p);
          list_destroy_2(estaciones[station_id]->andenes[platform_id]->pasajeros_premium, nuevo_primero_fila->value);
          estaciones[station_id]->andenes[platform_id]->pasajeros_premium = nuevo_primero_fila;
          break;
        }          
      }
      if (sente_a_todos==1)
      {
        nueva_lista = list_init(NULL);
        estaciones[station_id]->andenes[platform_id]->ultimo_en_fila_premium = nueva_lista;
        estaciones[station_id]->andenes[platform_id]->pasajeros_premium = nueva_lista;
      }
    }
    else if (string_equals(command, "PASAJERO"))
    {
      int station_id, platform_id, destination, category;
      fscanf(input_file, "%d %d %d %d", &station_id, &platform_id, &destination, &category);
      if (category == 1)
      {
        estaciones[station_id]->andenes[platform_id]->ultimo_en_fila_normal = list_append(estaciones[station_id]->andenes[platform_id]->pasajeros_normal, pasajero_init(contador_pasajeros, station_id, platform_id, destination, category), estaciones[station_id]->andenes[platform_id]->ultimo_en_fila_normal);
        contador_pasajeros++;
      }
      else if (category == 0)
      {
        if (estaciones[station_id]->andenes[platform_id]->tiene_tren == 0)
        {
          estaciones[station_id]->andenes[platform_id]->ultimo_en_fila_premium = list_append(estaciones[station_id]->andenes[platform_id]->pasajeros_premium, pasajero_init(contador_pasajeros, station_id, platform_id, destination, category), estaciones[station_id]->andenes[platform_id]->ultimo_en_fila_premium);
          contador_pasajeros++;
        }
        else
        {
          listo = 0;
          for (int v = 0; v < estaciones[station_id]->andenes[platform_id]->tren->cant_vagones;v++)
          {
            for (int a = 0; a < estaciones[station_id]->andenes[platform_id]->tren->vagones[v]->capacidad;a++)
            {
              if (estaciones[station_id]->andenes[platform_id]->tren->vagones[v]->asientos[a]->ocupado == 0)
              {
                estaciones[station_id]->andenes[platform_id]->tren->vagones[v]->asientos[a]->pasajero = pasajero_init(contador_pasajeros, station_id, platform_id, destination, category);
                contador_pasajeros++;
                listo++;
                estaciones[station_id]->andenes[platform_id]->tren->vagones[v]->asientos[a]->ocupado = 1;
                break;
              }
            }
            if (listo == 1)
            {
              break;
            }
          }
          if (listo==0)
          {
            estaciones[station_id]->andenes[platform_id]->ultimo_en_fila_premium = list_append(estaciones[station_id]->andenes[platform_id]->pasajeros_premium, pasajero_init(contador_pasajeros, station_id, platform_id, destination, category), estaciones[station_id]->andenes[platform_id]->ultimo_en_fila_premium);
            contador_pasajeros++;
          }
        }
      }
    }
    else if (string_equals(command, "REMOVER"))
    {
      int station_id, platform_id, car, seat;
      fscanf(input_file, "%d %d %d %d", &station_id, &platform_id, &car, &seat);
      int pasajero_id;
      int destino;
      pasajero_id = estaciones[station_id]->andenes[platform_id]->tren->vagones[car]->asientos[seat]->pasajero->id;
      destino = estaciones[station_id]->andenes[platform_id]->tren->vagones[car]->asientos[seat]->pasajero->destino;
      estaciones[station_id]->andenes[platform_id]->tren->vagones[car]->asientos[seat]->ocupado = 0;
      pasajero_destroy(estaciones[station_id]->andenes[platform_id]->tren->vagones[car]->asientos[seat]->pasajero);
      fprintf(output_file, "REMOVER %i %i\n", pasajero_id, destino);
      int largo_fila_premium;
      int sente_a_todos;
      sente_a_todos = 1;
      largo_fila_premium = list_last(estaciones[station_id]->andenes[platform_id]->pasajeros_premium, 0);
      for (int p=1;p<=largo_fila_premium;p++)
      {
        listo = 0;
        for (int v = 0; v < estaciones[station_id]->andenes[platform_id]->tren->cant_vagones;v++)
        {
          for (int a = 0; a < estaciones[station_id]->andenes[platform_id]->tren->vagones[v]->capacidad;a++)
          {
            if (estaciones[station_id]->andenes[platform_id]->tren->vagones[v]->asientos[a]->ocupado == 0)
            {
              estaciones[station_id]->andenes[platform_id]->tren->vagones[v]->asientos[a]->pasajero = list_at_index(estaciones[station_id]->andenes[platform_id]->pasajeros_premium, p);
              listo++;
              estaciones[station_id]->andenes[platform_id]->tren->vagones[v]->asientos[a]->ocupado = 1;
              break;
            }
          }
          if (listo == 1)
          {
            break;
          }
        }
        if (listo==0)
        {
          sente_a_todos = 0;
          nuevo_primero_fila = lista_en_lugar(estaciones[station_id]->andenes[platform_id]->pasajeros_premium, p);
          list_destroy_2(estaciones[station_id]->andenes[platform_id]->pasajeros_premium, nuevo_primero_fila->value);
          estaciones[station_id]->andenes[platform_id]->pasajeros_premium = nuevo_primero_fila;
          break;
        }          
      }
      if (sente_a_todos==1)
      {
        nueva_lista = list_init(NULL);
        estaciones[station_id]->andenes[platform_id]->ultimo_en_fila_premium = nueva_lista;
        estaciones[station_id]->andenes[platform_id]->pasajeros_premium = nueva_lista;
      }
    }
    else if (string_equals(command, "SALIR"))
    {
      int station_id, platform_id;
      fscanf(input_file, "%d %d", &station_id, &platform_id);

      int largo_fila_normal;
      int sente_a_todos;
      sente_a_todos = 1;
      largo_fila_normal = list_last(estaciones[station_id]->andenes[platform_id]->pasajeros_normal, 0);
      for (int p=1;p<=largo_fila_normal;p++)
      {
        listo = 0;
        for (int v = 0; v < estaciones[station_id]->andenes[platform_id]->tren->cant_vagones;v++)
        {
          for (int a = 0; a < estaciones[station_id]->andenes[platform_id]->tren->vagones[v]->capacidad;a++)
          {
            if (estaciones[station_id]->andenes[platform_id]->tren->vagones[v]->asientos[a]->ocupado == 0)
            {
              estaciones[station_id]->andenes[platform_id]->tren->vagones[v]->asientos[a]->pasajero = list_at_index(estaciones[station_id]->andenes[platform_id]->pasajeros_normal, p);
              listo++;
              estaciones[station_id]->andenes[platform_id]->tren->vagones[v]->asientos[a]->ocupado = 1;
              break;
            }
          }
          if (listo == 1)
          {
            break;
          }
        }
        if (listo==0)
        {
          nuevo_primero_fila = lista_en_lugar(estaciones[station_id]->andenes[platform_id]->pasajeros_normal, p);
          list_destroy_2(estaciones[station_id]->andenes[platform_id]->pasajeros_premium, nuevo_primero_fila->value);
          estaciones[station_id]->andenes[platform_id]->pasajeros_normal = nuevo_primero_fila;
          sente_a_todos = 0;
          break;
        }  
      }
      if (sente_a_todos==1)
      {
        nueva_lista = list_init(NULL);
        estaciones[station_id]->andenes[platform_id]->ultimo_en_fila_normal = nueva_lista;
        estaciones[station_id]->andenes[platform_id]->pasajeros_normal = nueva_lista;
      }     
      ultimo_que_salio = estaciones[station_id]->andenes[platform_id]->tren;
      estaciones[station_id]->andenes[platform_id]->tiene_tren = 0;
    }
    else if (string_equals(command, "LLEGAR"))
    {
      int station_id, platform_id;
      fscanf(input_file, "%d %d", &station_id, &platform_id);
      fprintf(output_file, "LLEGAR ");
      estado_tren(ultimo_que_salio, output_file);
      estaciones[station_id]->andenes[platform_id]->tiene_tren = 1;
      estaciones[station_id]->andenes[platform_id]->tren = ultimo_que_salio;
      for (int v=0; v<ultimo_que_salio->cant_vagones;v++)
      {
        for (int a=0; a<ultimo_que_salio->vagones[v]->capacidad;a++)
        {
          if (ultimo_que_salio->vagones[v]->asientos[a]->ocupado==1)
          {
            if (ultimo_que_salio->vagones[v]->asientos[a]->pasajero->destino == station_id)
            {
              ultimo_que_salio->vagones[v]->asientos[a]->ocupado=0;
            }
          }
        }
      }
      int sente_a_todos;
      sente_a_todos = 1;
      int largo_fila_premium;
      largo_fila_premium = list_last(estaciones[station_id]->andenes[platform_id]->pasajeros_premium, 0);
      for (int p=1;p<=largo_fila_premium;p++)
      {
        listo = 0;
        for (int v = 0; v < estaciones[station_id]->andenes[platform_id]->tren->cant_vagones;v++)
        {
          for (int a = 0; a < estaciones[station_id]->andenes[platform_id]->tren->vagones[v]->capacidad;a++)
          {
            if (estaciones[station_id]->andenes[platform_id]->tren->vagones[v]->asientos[a]->ocupado == 0)
            {
              estaciones[station_id]->andenes[platform_id]->tren->vagones[v]->asientos[a]->pasajero = list_at_index(estaciones[station_id]->andenes[platform_id]->pasajeros_premium, p);
              listo++;
              estaciones[station_id]->andenes[platform_id]->tren->vagones[v]->asientos[a]->ocupado = 1;
              break;
            }
          }
          if (listo == 1)
          {
            break;
          }
        }
        if (listo==0)
        {
          sente_a_todos = 0;
          nuevo_primero_fila = lista_en_lugar(estaciones[station_id]->andenes[platform_id]->pasajeros_premium, p);
          list_destroy_2(estaciones[station_id]->andenes[platform_id]->pasajeros_premium, nuevo_primero_fila->value);
          estaciones[station_id]->andenes[platform_id]->pasajeros_premium = nuevo_primero_fila;
          break;
        }          
      }
      if (sente_a_todos==1)
      {
        nueva_lista = list_init(NULL);
        estaciones[station_id]->andenes[platform_id]->ultimo_en_fila_premium = nueva_lista;
        estaciones[station_id]->andenes[platform_id]->pasajeros_premium = nueva_lista;
      }
    }
    else if (string_equals(command, "DESAPARECER"))
    {
      fprintf(output_file, "DESAPARECER ");
      estado_tren(ultimo_que_salio, output_file);
      for (int v=0; v<ultimo_que_salio->cant_vagones;v++)
      {
        for (int as=0;as<ultimo_que_salio->vagones[v]->capacidad;as++)
        {
          if (ultimo_que_salio->vagones[v]->asientos[as]->ocupado == 1)
          {
            pasajero_destroy(ultimo_que_salio->vagones[v]->asientos[as]->pasajero);
          }
          asiento_destroy(ultimo_que_salio->vagones[v]->asientos[as]);
        }
        vagon_destroy(ultimo_que_salio->vagones[v]);
      }
      tren_destroy(ultimo_que_salio);
    }
    else if (string_equals(command, "SEPARAR"))
    {
      int station_id_1, platform_id_1, station_id_2, platform_id_2;
      fscanf(input_file, "%d %d %d %d", &station_id_1, &platform_id_1, &station_id_2, &platform_id_2);
      vagones_separar_1 = 0;
      vagones_separar_2 = 0;
      for (int v=0;v<ultimo_que_salio->cant_vagones;v++)
      {
        if (v%2==0)
        {
          vagones_separar_1++;
        }
        else if (v%2 != 0)
        {
          vagones_separar_2++;
        }
      }
      separar_1 = tren_init(ultimo_que_salio->id, vagones_separar_1);
      separar_2 = tren_init(contador_trenes, vagones_separar_2);
      contador_trenes++;
      cuenta_separar_1 = 0;
      cuenta_separar_2 = 0;
      for (int v=0; v<ultimo_que_salio->cant_vagones;v++)
      {
        if (v%2==0)
        {
          separar_1->vagones[cuenta_separar_1] = ultimo_que_salio->vagones[v];
          cuenta_separar_1++;
        }
        else if (v%2!=0)
        {
          separar_2->vagones[cuenta_separar_2] = ultimo_que_salio->vagones[v];
          cuenta_separar_2++;
        }
      }
      fprintf(output_file, "LLEGAR ");
      estado_tren(separar_1, output_file);
      estaciones[station_id_1]->andenes[platform_id_1]->tiene_tren = 1;
      estaciones[station_id_1]->andenes[platform_id_1]->tren = separar_1;
      for (int v=0; v<separar_1->cant_vagones;v++)
      {
        for (int a=0; a<separar_1->vagones[v]->capacidad;a++)
        {
          if (separar_1->vagones[v]->asientos[a]->ocupado==1)
          {
            if (separar_1->vagones[v]->asientos[a]->pasajero->destino == station_id_1)
            {
              separar_1->vagones[v]->asientos[a]->ocupado=0;
            }
          }
        }
      }
      int sente_a_todos;
      sente_a_todos = 1;
      int largo_fila_premium;
      largo_fila_premium = list_last(estaciones[station_id_1]->andenes[platform_id_1]->pasajeros_premium, 0);
      for (int p=1;p<=largo_fila_premium;p++)
      {
        listo = 0;
        for (int v = 0; v < estaciones[station_id_1]->andenes[platform_id_1]->tren->cant_vagones;v++)
        {
          for (int a = 0; a < estaciones[station_id_1]->andenes[platform_id_1]->tren->vagones[v]->capacidad;a++)
          {
            if (estaciones[station_id_1]->andenes[platform_id_1]->tren->vagones[v]->asientos[a]->ocupado == 0)
            {
              estaciones[station_id_1]->andenes[platform_id_1]->tren->vagones[v]->asientos[a]->pasajero = list_at_index(estaciones[station_id_1]->andenes[platform_id_1]->pasajeros_premium, p);
              listo++;
              estaciones[station_id_1]->andenes[platform_id_1]->tren->vagones[v]->asientos[a]->ocupado = 1;
              break;
            }
          }
          if (listo == 1)
          {
            break;
          }
        }
        if (listo==0)
        {
          sente_a_todos = 0;
          nuevo_primero_fila = lista_en_lugar(estaciones[station_id_1]->andenes[platform_id_1]->pasajeros_premium, p);
          list_destroy_2(estaciones[station_id_1]->andenes[platform_id_1]->pasajeros_premium, nuevo_primero_fila->value);
          estaciones[station_id_1]->andenes[platform_id_1]->pasajeros_premium = nuevo_primero_fila;
          break;
        }          
      }
      if (sente_a_todos==1)
      {
        estaciones[station_id_1]->andenes[platform_id_1]->pasajeros_premium = list_init(NULL);
      }
      fprintf(output_file, "LLEGAR ");
      estado_tren(separar_2, output_file);
      estaciones[station_id_2]->andenes[platform_id_2]->tiene_tren = 1;
      estaciones[station_id_2]->andenes[platform_id_2]->tren = separar_2;
      for (int v=0; v<separar_2->cant_vagones;v++)
      {
        for (int a=0; a<separar_2->vagones[v]->capacidad;a++)
        {
          if (separar_2->vagones[v]->asientos[a]->ocupado==1)
          {
            if (separar_2->vagones[v]->asientos[a]->pasajero->destino == station_id_2)
            {
              separar_2->vagones[v]->asientos[a]->ocupado=0;
            }
          }
        }
      }
      sente_a_todos = 1;
      largo_fila_premium = list_last(estaciones[station_id_2]->andenes[platform_id_2]->pasajeros_premium, 0);
      for (int p=1;p<=largo_fila_premium;p++)
      {
        listo = 0;
        for (int v = 0; v < estaciones[station_id_2]->andenes[platform_id_2]->tren->cant_vagones;v++)
        {
          for (int a = 0; a < estaciones[station_id_2]->andenes[platform_id_2]->tren->vagones[v]->capacidad;a++)
          {
            if (estaciones[station_id_2]->andenes[platform_id_2]->tren->vagones[v]->asientos[a]->ocupado == 0)
            {
              estaciones[station_id_2]->andenes[platform_id_2]->tren->vagones[v]->asientos[a]->pasajero = list_at_index(estaciones[station_id_2]->andenes[platform_id_2]->pasajeros_premium, p);
              listo++;
              estaciones[station_id_2]->andenes[platform_id_2]->tren->vagones[v]->asientos[a]->ocupado = 1;
              break;
            }
          }
          if (listo == 1)
          {
            break;
          }
        }
        if (listo==0)
        {
          sente_a_todos = 0;
          nuevo_primero_fila = lista_en_lugar(estaciones[station_id_2]->andenes[platform_id_2]->pasajeros_premium, p);
          list_destroy_2(estaciones[station_id_2]->andenes[platform_id_2]->pasajeros_premium, nuevo_primero_fila->value);
          estaciones[station_id_2]->andenes[platform_id_2]->pasajeros_premium = nuevo_primero_fila;
          break;
        }          
      }
      if (sente_a_todos==1)
      {
        estaciones[station_id_2]->andenes[platform_id_2]->pasajeros_premium = list_init(NULL);
      }
    }
    else if (string_equals(command, "ESTACION"))
    {
      int station_id;
      fscanf(input_file, "%d", &station_id);
      fprintf(output_file, "ESTACION %i\n", station_id);
      for (int a = 0; a < estaciones[station_id]->cant_andenes;a++)
      {
        fprintf(output_file, "A%i ", estaciones[station_id]->andenes[a]->id);
        list_print(estaciones[station_id]->andenes[a]->pasajeros_premium, 1, output_file);
        list_print(estaciones[station_id]->andenes[a]->pasajeros_normal, 1, output_file);
        fprintf(output_file, "\n");
        if (estaciones[station_id]->andenes[a]->tiene_tren == 1)
        {
          estado_tren(estaciones[station_id]->andenes[a]->tren, output_file);
        }
      }
    }

    /* Leemos la siguiente instrucción */
    fscanf(input_file, "%s", command);
  }

  /*  [Por implementar] Liberamos nuestras estructuras */
  for (int e=0; e<N_STATIONS;e++)
  {
    fprintf(output_file, "ESTACION %i\n", estaciones[e]->id);
    for (int a = 0; a < estaciones[e]->cant_andenes;a++)
    {
      fprintf(output_file, "A%i ", estaciones[e]->andenes[a]->id);
      list_print(estaciones[e]->andenes[a]->pasajeros_premium, 1, output_file);
      list_print(estaciones[e]->andenes[a]->pasajeros_normal, 1, output_file);
      fprintf(output_file, "\n");
      if (estaciones[e]->andenes[a]->tiene_tren == 1)
      {
        estado_tren(estaciones[e]->andenes[a]->tren, output_file);
      }
    }
  }

  for (int e = 0;e<N_STATIONS;e++)
  {
    for (int a=0;a<estaciones[e]->cant_andenes;a++)
    {
      if (estaciones[e]->andenes[a]->tiene_tren == 1)
      {
        for (int v=0; v<estaciones[e]->andenes[a]->tren->cant_vagones;v++)
        {
          for (int as=0;as<estaciones[e]->andenes[a]->tren->vagones[v]->capacidad;as++)
          {
            if (estaciones[e]->andenes[a]->tren->vagones[v]->asientos[as]->ocupado == 1)
            {
              pasajero_destroy(estaciones[e]->andenes[a]->tren->vagones[v]->asientos[as]->pasajero);
            }
            asiento_destroy(estaciones[e]->andenes[a]->tren->vagones[v]->asientos[as]);
          }
          vagon_destroy(estaciones[e]->andenes[a]->tren->vagones[v]);
        }
        if (estaciones[e]->andenes[a]->tiene_tren == 1)
        {
          tren_destroy(estaciones[e]->andenes[a]->tren);
        }
      }
      anden_destroy(estaciones[e]->andenes[a]);
    }
    estacion_destroy(estaciones[e]);
  }
  free(estaciones);
  fclose(input_file);
  fclose(output_file);
  return 0;
}