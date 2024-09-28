#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <time.h>
#include <stdbool.h>

#include "RGB_Couleur.h"

#define PGM 1

/*
    Prend comme parametre un pixel en RGB et renvoie sa valeur en YCbCr
    La valeur retourné est un pointeur vers la structure pixel_YCbCr
*/

void free_MCU_YCbCr(struct MCU_YCbCr MCU, struct fact_echant *fact1, struct fact_echant *fact2)
{   
    // for (uint8_t l = 0; l<(fact1->h*fact1->v); l++){
    //     free_matrix(MCU.blocs[l].Y, 8);
    // }
    // for (uint8_t l = 0; l<fact2->h*fact2->v; l++){
    //     free_matrix(MCU.blocs[l].Cb, 8);
    //     free_matrix(MCU.blocs[l].Cr, 8);
    // }
    // free(MCU.blocs);
}



struct pixel_Y *RGB_to_YCbCr_Couleur( struct pixel_R *pixel ){ // validé 

    uint8_t red = pixel->R;
    uint8_t green = pixel->G;
    uint8_t blue = pixel->B;

    struct pixel_Y *nv_pixel = malloc(sizeof(struct pixel_Y));
    nv_pixel->Y = (uint16_t) roundf(0.299*red + 0.587*green + 0.114*blue);
    nv_pixel->Cb = (uint16_t) roundf(-0.1687*red - 0.3313*green + 0.5*blue + 128);
    nv_pixel->Cr = (uint16_t) roundf(0.5*red - 0.4187*green - 0.0813*blue + 128);

    return nv_pixel;

}


/*
    Cette fonction prend comme parametre un pointeur vers une matrice MCU de taille 8x8
    er retourne un pointeur vers la matrice YCbCr équivalente
*/

struct Matrix_YCbCr *conversion_Couleur( struct MCU_s_C * matrice_initiale){

    struct  Matrix_YCbCr *nw_matrix = malloc(sizeof(struct Matrix_YCbCr)); // notre nouvelle matrice qu'on va retourner
    //struct pixel_R **matrice = matrice_initiale->matrix; // notre matrice qui contient les données 
    struct pixel_R pixel_actu; // sert au parcours des pixels
    struct pixel_Y nv_pixel; // pixel qui contient les composantes Y,Cb et Cr

    nw_matrix->Y = calloc(8, sizeof(uint16_t *)); // on alloue la mémoire pour les matrices Y, Cb et Cr
    nw_matrix->Cb = calloc(8, sizeof(uint16_t *));
    nw_matrix->Cr = calloc(8, sizeof(uint16_t *));
    //La libération de ces allocations pose un problème. Je les libère dans la fonction free_MCU_YCbCr à travers free_matrix
    //qui libère les MCU.blocs[l] exsistants

    for ( uint8_t q = 0; q<8; q++){
        (nw_matrix->Y)[q] = calloc(8, sizeof(uint16_t)); // on alloue la mémoire pour les lignes des matrices
        (nw_matrix->Cb)[q] = calloc(8, sizeof(uint16_t));
        (nw_matrix->Cr)[q] = calloc(8, sizeof(uint16_t));
        
        for (uint8_t s = 0; s< 8 ;s++){
            struct pixel_Y *pix_Y = RGB_to_YCbCr_Couleur(matrice_initiale->matrix[q][s]);
            nv_pixel = *pix_Y; // On calcule la valeur des composantes Y Cb et Cr
            
            (nw_matrix->Y)[q][s] = nv_pixel.Y;  // On les stock
            (nw_matrix->Cb)[q][s] = nv_pixel.Cb;
            (nw_matrix->Cr)[q][s] = nv_pixel.Cr;
            free(pix_Y);
        }
    }
    //On ne peut pas utiliser free_MCU_Couleur car cette fonction libère l'espace alloué pour chaque structure pixel, il a déjà été libéré dans
    //le premier appel concernant matrice_brute.
    for (uint16_t j =0; j<8; j++){
        free(matrice_initiale->matrix[j]);
    }
    free(matrice_initiale->matrix);
    return nw_matrix;
    
}

/*
    POur commencer cette fonction n'est valide que pour le niv de gris 

    Elle prend comme parametre une matrice des composants Y ( un seul bloc 8x8 ) et elle retourne une matrice YCbCr avec juste la composante Y qui est 
    pleine ( ce qui nous suffit ici car c'est la seul composante avec laquelle on travail)
    En vrai elle ne fait rien elle change juste de structure 
*/

// struct Matrix_YCbCr *conversion_pgm( int16_t ** matrice_initiale){

//     struct  Matrix_YCbCr *nw_matrix = malloc(sizeof(struct Matrix_YCbCr)); // notre nouvelle matrice qu'on va retourner
//     int16_t **matrice = matrice_initiale; // notre matrice qui contient les données 


//     nw_matrix->Y = malloc(8*sizeof(int16_t*)); // on alloue la mémoire pour les matrices Y, Cb et Cr
//     nw_matrix->Cb = NULL;
//     nw_matrix->Cr = NULL;

//     for ( uint8_t i = 0; i<8; i++){
//         (nw_matrix->Y)[i] = malloc(8*sizeof(int16_t)); // on alloue la mémoire pour les lignes des matrices
 
        
//         for (uint8_t j = 0; j< 8 ;j++){
            
//             (nw_matrix->Y)[i][j] = (int16_t) matrice_initiale[i][j];  // On les stock

//         }
//     }

//     return nw_matrix;
    

// }


/*
    Elle prend la matrice des MCUs et renvoie un pointeur vers une MCU avec maintenant les composantes YCbCr
*/

struct MCU_YCbCr *MCU_to_YCbCr_Couleur(struct MCU_s_C *MCU, struct fact_echant *fact_1, struct fact_echant *fact2, struct fact_echant *fact3) //validé  //changement //changement
{
    struct MCU_YCbCr *MCU_YCbCr = malloc(sizeof(struct MCU_YCbCr));
    MCU_YCbCr->blocs = calloc(MCU->bloc_col*MCU->bloc_line, sizeof(struct Matrix_YCbCr));
    MCU_YCbCr->col = MCU->bloc_col;
    MCU_YCbCr->lines = MCU->bloc_line;
    if (MCU->blocs != NULL){
        for (uint8_t i = 0; i<MCU->bloc_col*MCU->bloc_line; i++){
            struct Matrix_YCbCr *converti = conversion_Couleur(MCU->blocs[i]);
            MCU_YCbCr->blocs[i] = *converti;
            free(MCU->blocs[i]);
            free(converti);
        }    
        free(MCU->blocs);
    }
    else{
        struct Matrix_YCbCr *converti = conversion_Couleur(MCU);
        MCU_YCbCr->blocs[0] = *converti;
        free(converti);
    }
    sous_echantillonage(MCU_YCbCr, fact_1, fact2, fact3);
    return MCU_YCbCr;
}

/*
    Elle prend une matrice MCU cad une matrice des MCU apres découpage en RGB ou bien avec une seul composnate Y, et retourne une matrice avec 
    des MCU contenant les composantes YCbCr 
*/


struct matrix_MCU_YCbCr *matrix_MCU_to_YCbCr_Couleur(struct matrix_MCU_C matrix_MCU, struct fact_echant *fact_1, struct fact_echant *fact2, struct fact_echant *fact3) // validé normalement    // changement //changement
{
    struct matrix_MCU_YCbCr *matrix_MCU_YCbCr = malloc(sizeof(struct matrix_MCU_YCbCr));
    matrix_MCU_YCbCr->bloc_col = matrix_MCU.MCU_col;
    matrix_MCU_YCbCr->bloc_line = matrix_MCU.MCU_lines;
    matrix_MCU_YCbCr->matrice = calloc(matrix_MCU_YCbCr->bloc_line, sizeof(struct MCU_YCbCr *));
    for (uint32_t i = 0; i<matrix_MCU_YCbCr->bloc_line; i++){
        matrix_MCU_YCbCr->matrice[i] = calloc(matrix_MCU_YCbCr->bloc_col, sizeof(struct MCU_YCbCr));
        for (uint32_t j = 0; j<matrix_MCU_YCbCr->bloc_col; j++){
            matrix_MCU_YCbCr->matrice[i][j].col = fact_1->v;
            matrix_MCU_YCbCr->matrice[i][j].lines = fact_1->h;
            struct MCU_YCbCr *mcu_ycbcr = MCU_to_YCbCr_Couleur(&(matrix_MCU.matrice[i][j]), fact_1, fact2, fact3);
            matrix_MCU_YCbCr->matrice[i][j] = *mcu_ycbcr;
            free(mcu_ycbcr);
        }
        free(matrix_MCU.matrice[i]);
    }
    return matrix_MCU_YCbCr;
}



/*int main(){
    // Création de la matrice de pixel ayant RGB
    //notre_matrice->bloc_col = 8;
    //notre_matrice->bloc_line = 8;
    //notre_matrice->matrix = malloc(16*sizeof(struct pixel_R*));
    struct pixel_R **matrice_pixel = calloc(27, sizeof(struct pixel_R *));
    for (uint8_t i=0; i<27; i++){
        matrice_pixel[i] = calloc(17, sizeof(struct pixel_R));
        for (uint8_t j=0; j<17; j++){
            matrice_pixel[i][j].R = rand() % 256;
            matrice_pixel[i][j].G = rand() % 256;
            matrice_pixel[i][j].B = rand() % 256;
        }
    }
    affiche_matrice_Couleur(matrice_pixel, 17, 27);
    struct matrix_C matrixx = {matrice_pixel, 17, 27};
    //struct MCU_s_C *notre_matrice = creer_MCU_Couleur(matrice_pixel, 2,2);
    //struct matrix_C *matrice_decoupe = decoupe_matrice_Couleur(matrixx, 3, 4);
    struct matrix_MCU_C img =  image_to_MCU_Couleur(matrixx, 1, 1);
    // // génération de la nouvelle matrice avec les composantes YCbCr
    // //struct Matrix_YCbCr *nv_matrice = conversion(notre_matrice);
    // //struct MCU_YCbCr *matrice_YCbCr = MCU_to_YCbCr(notre_matrice);
    // //struct matrix_MCU_YCbCr *matrixx_YCbCr = matrix_MCU_to_YCbCr(img);
    for (uint8_t i=0; i<8; i++){
        for (uint8_t j=0; j<8; j++){
            printf("%x \t", img.matrice[3][2].matrix[i][j].R);
        }
        printf("\n");
    }
}*/