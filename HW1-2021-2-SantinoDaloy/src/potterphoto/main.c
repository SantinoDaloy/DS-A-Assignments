#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "tree.h"
#include "alpha.h"
#include "../imagelib/image.h"

void invert(Image* image){
    int aux[128]; 
    for (int i = 0; i < 128; i++){
        aux[i] = 127 - i;
    }
    for (int i = 0; i < image->pixel_count; i++){
        image->pixels[i] = aux[image->pixels[i]];
    }
}

int main(int argc, char** argv)
{
    // Revisamos los argumentos
    if(argc < 5) {
        printf("Modo de uso: %s <input.png> <output.png> <command> [args]\n", argv[0]);
        return 1;
    }

    // Cargamos la imagen original desde el archivo
    Image* image = img_png_read_from_file(argv[1]);
    Image* marker = img_png_read_from_file(argv[3]);
    /* ------------- Parametro R opcional, invierte la imagen -------------- */
    if (argc > 5){
        invert(image);
    }
    //printf("pixel count: %i \n", image->width);
    //printf("pixel count: %i \n", image->height);

    /* ------------- POR IMPLEMENTAR -------------- */
    /* Aqui debes crear el ComponentTree de la imagen.*/
    int total_nodos = 1;
    Component_tree_node* root_node;
    Queue* queue_childs;
    Queue* backup;
    root_node = tree_node_init(0, image->pixel_count-1);
    root_node->cant_usada = (image->pixel_count-1);
    queue_childs = queue_init(root_node);
    queue_childs->tail = queue_childs;
    queue_childs->value = root_node;
    backup = queue_childs;
    int A[image->pixel_count];
    for (int p=0;p<image->pixel_count;p++){
        A[p]=p;
    }
    root_node->pixels = A;
    if (!queue_childs->next){
        create_childs(queue_childs->value, image, queue_childs);
        total_nodos += queue_childs->value->cant_hijos;
    }
    while (queue_childs->next){
        queue_childs->next->tail = queue_childs->tail;
        queue_childs = queue_childs->next;
        create_childs(queue_childs->value, image, queue_childs);
        total_nodos += queue_childs->value->cant_hijos;
    }
    printf("total nodos es: %i\n", total_nodos);
    // Creamos una nueva imagen de igual tamaño, para el output
    Image* new_img = calloc(1, sizeof(Image));
    *new_img = (Image) {
        .height = image->height,
        .width = image->width,
        .pixel_count = image->pixel_count,
        .pixels = calloc(image->pixel_count, sizeof(int))
    };

    /* ------- Leemos el filtro alpha  --------      */  
   float alpha = atof(argv[4]); 

    /* ------------- POR IMPLEMENTAR -------------- */
    /* Aqui debes implementar el filtro Alpha y     */
    /* guardar la imagen filtrada en new_img.       */
    int cuenta_alpha = 0;
    //Queue* backup2 = backup;
    Alpha_filter* filter = filter_init(total_nodos);
    while (backup->next){
        filter->nodos[cuenta_alpha] = backup->value;
        cuenta_alpha++;
        backup->next->tail = backup->tail;
        backup = backup->next;
    }
    if (!backup->next){
        filter->nodos[cuenta_alpha] = backup->value;
    }
    //queue_destroy(backup2);
    recorrer_nodos(filter, marker, image, alpha);
    Queue* analizar = root_node->incluidos_momento;
    if (!analizar->next){
        if (analizar->value != NULL){
            for (int p=0;p<analizar->value->cant_usada;p++){
                new_img->pixels[analizar->value->pixels[p]] = image->pixels[analizar->value->pixels[p]];
            }
        }
    }
    while (analizar->next){
        if (analizar->value == NULL){
            analizar = analizar->next;
        }
        else{
            for (int p=0;p<analizar->value->cant_usada;p++){
                new_img->pixels[analizar->value->pixels[p]] = image->pixels[analizar->value->pixels[p]];
            }
            analizar = analizar->next;
        }
    }
    if (analizar->value != NULL){
        for (int p=0;p<analizar->value->cant_usada;p++){
            new_img->pixels[analizar->value->pixels[p]] = image->pixels[analizar->value->pixels[p]];
        }
    }



    /* Se vuelve a invertir la imagen para dejarla como la original  */
    if (argc > 5){
        invert(new_img);
    }    

    // Exportamos la nueva imagen
    printf("Guardando imagen en %s\n", argv[2]);
    img_png_write_to_file(new_img, argv[2]);
    printf("Listo!\n");

    // Liberamos los recursos
    img_png_destroy(image);
    img_png_destroy(new_img);
    img_png_destroy(marker);

    // Terminamos exitosamente
    return 0;
}