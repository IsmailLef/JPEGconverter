#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>

#include "RGB.h"

#define PGM 1

// struct pixel_R{
//     // on a juste des valeurs positifs donc on travaille avec des unsigned
//     uint8_t R;
//     uint8_t G;
//     uint8_t B;
// };

// struct matrix {
//     int8_t **matrice;
//     int col;
//     int lines;
// };

// struct MCU_s {
//     uint8_t bloc_col;
//     uint8_t bloc_line;
//     struct MCU_s **blocs; // un pointeur vers les blocks de la marice
//     struct pixel_R **matrix; // NULL quand on a une matrice autre que 8x8, et contient la matrice 8x8  si c'est sa taille
// };

// struct Matrix_YCbCr{
//     uint8_t **Y;
//     uint8_t  **Cb;
//     uint8_t **Cr;
// };

// struct pixel_Y{
//     // on a fait des int car les valeurs peuvent être négatif
//     uint8_t Y;
//     uint8_t Cb;
//     uint8_t Cr;
// };

// struct matrix_MCU_YCbCr{
//     //Convertit la matrice des MCU_s correspondante à l'image en MCU_s YCbCr
//     struct MCU_YCbCr **matrice;
//     uint32_t bloc_col;
//     uint32_t bloc_line;
// };

/*
    Prend comme parametre un pixel en RGB et renvoie sa valeur en YCbCr
    La valeur retourné est un pointeur vers la structure pixel_YCbCr
*/

struct pixel_Y *RGB_to_YCbCr( struct pixel_R *pixel ){

    uint8_t red = pixel->R;
    uint8_t green = pixel->G;
    uint8_t blue = pixel->B;

    struct pixel_Y *nv_pixel = malloc(sizeof(struct pixel_Y));
    nv_pixel->Y = (int16_t) (0.299*red + 0.587*green + 0.114*blue);
    nv_pixel->Cb = (int16_t) (-0.1687*red - 0.3313*green + 0.5*blue + 128);
    nv_pixel->Cr = (int16_t) (0.5*red - 0.4187*green - 0.0813*blue + 128);

    return nv_pixel;

}


/*
    Cette fonction prend comme parametre un pointeur vers une matrice MCU de taille 8x8
    er retourne un pointeur vers la matrice YCbCr équivalente
*/

struct Matrix_YCbCr *conversion( struct MCU_s *matrice_initiale){

    struct  Matrix_YCbCr *nw_matrix = malloc(sizeof(struct Matrix_YCbCr)); // notre nouvelle matrice qu'on va retourner
    struct pixel_R **matrice = matrice_initiale->matrix; // notre matrice qui contient les données 
    struct pixel_R pixel_actu; // sert au parcours des pixels
    struct pixel_Y nv_pixel; // pixel qui contient les composantes Y,Cb et Cr

    nw_matrix->Y = malloc(8*sizeof(int8_t*)); // on alloue la mémoire pour les matrices Y, Cb et Cr
    nw_matrix->Cb = malloc(8*sizeof(int8_t*));
    nw_matrix->Cr = malloc(8*sizeof(int8_t*));

    for ( uint8_t i = 0; i<8; i++){
        (nw_matrix->Y)[i] = malloc(8*sizeof(int8_t)); // on alloue la mémoire pour les lignes des matrices
        (nw_matrix->Cb)[i] = malloc(8*sizeof(int8_t));
        (nw_matrix->Cr)[i] = malloc(8*sizeof(int8_t));
        
        for (uint8_t j = 0; j< 8 ;j++){
            nv_pixel = *RGB_to_YCbCr(&(matrice[i][j])); // On calcule la valeur des composantes Y Cb et Cr
            
            (nw_matrix->Y)[i][j] = (int16_t) nv_pixel.Y;  // On les stock
            (nw_matrix->Cb)[i][j] = (int16_t) nv_pixel.Cb;
            (nw_matrix->Cr)[i][j] = (int16_t) nv_pixel.Cr;
        }
    }
    free_MCU(*matrice_initiale);
    return nw_matrix;
    
}

/*
    POur commencer cette fonction n'est valide que pour le niv de gris 

    Elle prend comme parametre une matrice des composants Y ( un seul bloc 8x8 ) et elle retourne une matrice YCbCr avec juste la composante Y qui est 
    pleine ( ce qui nous suffit ici car c'est la seul composante avec laquelle on travail)
    En vrai elle ne fait rien elle change juste de structure 
*/

struct Matrix_YCbCr *conversion_pgm( int16_t ** matrice_initiale){

    struct  Matrix_YCbCr *nw_matrix = malloc(sizeof(struct Matrix_YCbCr)); // notre nouvelle matrice qu'on va retourner
    int16_t **matrice = matrice_initiale; // notre matrice qui contient les données 


    nw_matrix->Y = malloc(8*sizeof(int16_t*)); // on alloue la mémoire pour les matrices Y, Cb et Cr
    nw_matrix->Cb = NULL;
    nw_matrix->Cr = NULL;

    for ( uint8_t i = 0; i<8; i++){
        (nw_matrix->Y)[i] = malloc(8*sizeof(int16_t)); // on alloue la mémoire pour les lignes des matrices
 
        
        for (uint8_t j = 0; j< 8 ;j++){
            
            (nw_matrix->Y)[i][j] = (int16_t) matrice_initiale[i][j];  // On les stock

        }
    }
    free_matrix(matrice_initiale, 8);
    return nw_matrix;
}


/*
    Elle prend la matrice des MCUs et renvoie un pointeur vers une MCU avec maintenant les composantes YCbCr
*/

struct MCU_YCbCr *MCU_to_YCbCr(struct MCU_s *MCU)
{// pgm est true si l'image est pgm
    if (PGM == 0){
    struct MCU_YCbCr *MCU_YCbCr = malloc(sizeof(struct MCU_YCbCr));
    MCU_YCbCr->blocs = calloc(MCU->bloc_col*MCU->bloc_line, sizeof(struct Matrix_YCbCr));
    MCU_YCbCr->col = MCU->bloc_col;
    MCU_YCbCr->lines = MCU->bloc_line;
    if (MCU->blocs != NULL){
        for (uint8_t i = 0; i<MCU->bloc_col*MCU->bloc_line; i++){
            struct Matrix_YCbCr *converti = conversion(MCU->blocs[i]);
            MCU_YCbCr->blocs[i] = *converti;
            free(converti);
        }    
    }
    else{
        struct Matrix_YCbCr *converti = conversion(MCU);
        MCU_YCbCr->blocs[0] = *converti; // j'ai pas compris ce else
        free(converti);
    }
    return MCU_YCbCr;

    }
    else {
    struct MCU_YCbCr *MCU_YCbCr = malloc(sizeof(struct MCU_YCbCr));
    MCU_YCbCr->blocs = calloc(MCU->bloc_col*MCU->bloc_line, sizeof(struct Matrix_YCbCr));
    MCU_YCbCr->col = MCU->bloc_col;
    MCU_YCbCr->lines = MCU->bloc_line;
    if (MCU->blocs != NULL){
        for (uint8_t i = 0; i<MCU->bloc_col*MCU->bloc_line; i++){
            struct Matrix_YCbCr *converti = conversion_pgm(MCU->blocs[i]->matrix);
            MCU_YCbCr->blocs[i] = *converti;
            free(converti);
        }    
    }
    else{
        struct Matrix_YCbCr *converti = conversion_pgm(MCU->matrix);
        MCU_YCbCr->blocs[0] = *converti; // ce else aussi je l'ai pas compris
        free(converti);
        }
    return MCU_YCbCr;
    }
    
}

/*
    Elle prend une matrice MCU cad une matrice des MCU apres découpage en RGB ou bien avec une seul composnate Y, et retourne une matrice avec 
    des MCU contenant les composantes YCbCr 
*/


struct matrix_MCU_YCbCr *matrix_MCU_to_YCbCr(struct matrix_MCU matrix_MCU)
{
    struct matrix_MCU_YCbCr *matrix_MCU_YCbCr = malloc(sizeof(struct matrix_MCU_YCbCr));
    matrix_MCU_YCbCr->bloc_col = matrix_MCU.MCU_col;
    matrix_MCU_YCbCr->bloc_line = matrix_MCU.MCU_lines;
    matrix_MCU_YCbCr->matrice = calloc(matrix_MCU_YCbCr->bloc_line, sizeof(struct MCU_YCbCr *));
    for (uint32_t i = 0; i<matrix_MCU_YCbCr->bloc_line; i++){
        matrix_MCU_YCbCr->matrice[i] = calloc(matrix_MCU_YCbCr->bloc_col, sizeof(struct MCU_YCbCr));
        for (uint32_t j = 0; j<matrix_MCU_YCbCr->bloc_col; j++){
            struct MCU_YCbCr *mcu_ycbcr = MCU_to_YCbCr(&(matrix_MCU.matrice[i][j]));
            matrix_MCU_YCbCr->matrice[i][j] = *mcu_ycbcr;
            free(mcu_ycbcr);
        }
    }
    return matrix_MCU_YCbCr;
}



/*int main(){
    // Création de la matrice de pixel ayant RGB
    //notre_matrice->bloc_col = 8;
    //notre_matrice->bloc_line = 8;
    //notre_matrice->matrix = malloc(16*sizeof(struct pixel_R*));
    int16_t **matric_pixel = malloc(16*sizeof(int16_t *));
    for (uint8_t i=0; i<27; i++){
        matric_pixel[i] = malloc(16*sizeof(int16_t));
        for (uint8_t j=0; j<17; j++){
            matric_pixel[i][j] = rand() % 256;
            // matric_pixel[i][j].G = rand() % 256;
            // matric_pixel[i][j].B = rand() % 256;
        }
    }
    affiche_matrice(matric_pixel, 17, 27);
    struct matrix matrixx = {matric_pixel, 17, 27};
    //struct MCU_s *notre_matrice = creer_MCU(matric_pixel, 2,2);
    struct matrix_MCU img =  image_to_MCU(matrixx, 1, 1);
    // génération de la nouvelle matrice avec les composantes YCbCr
    //struct Matrix_YCbCr *nv_matrice = conversion(notre_matrice);
    //struct MCU_YCbCr *matrice_YCbCr = MCU_to_YCbCr(notre_matrice);
    struct matrix_MCU_YCbCr *matrixx_YCbCr = matrix_MCU_to_YCbCr(img);
    for (uint8_t i=0; i<8; i++){
        for (uint8_t j=0; j<8; j++){
            printf("%04hx \t", matrixx_YCbCr->matrice[0][1].blocs[0].Y[i][j]);
        }
        printf("\n");
    }
}*/