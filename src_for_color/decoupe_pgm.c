#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "decoupe_pgm.h"
#include "RGB.h"


//  ########## COM DE MOURAD
// J'AI CHANGÉ TOUS LES INT EN UINT32_T CAR J'AI EU LA FLEMME DE REVOIR EXACTEMENT OU IL FAUT UINT8_T OU IL FAUT INT ET OU IL FAUT UINT8_T



// struct matrix {
//     int16_t **matrice;
//     uint32_t col;
//     uint32_t lines;
// };

// struct matrix_MCU{
//     struct MCU_s **matrice;
//     uint32_t MCU_col;
//     uint32_t MCU_lines;
// };


// struct MCU_s {
//     uint8_t bloc_col;
//     uint8_t bloc_line;
//     struct MCU_s **blocs;
//     int16_t **matrix;
// };

/*
    Permet d'afficher une matrice en donnant le nombre de lignes et de colonnes
*/


void affiche_matrice(int16_t **matrixx, uint32_t col, uint32_t line)
{
    for (uint32_t i = 0; i<line; i++){
        for (uint32_t j = 0; j<col; j++) printf("%04hx     ", matrixx[i][j]);
        printf(" \n");
    }
}

/*  
    Permet de free une matrice en donnant le nombre de lignes
*/

void free_matrix(int16_t **matrix, uint8_t lines)
{
    for (uint8_t i = 0; i<lines; i++){
        free(matrix[i]);
    }
    free(matrix);
}


/*
    Permet de free une seul MCU
*/

void free_MCU(struct MCU_s MCU)
{   
    if (MCU.blocs == NULL){
        free_matrix(MCU.matrix, 8);
    }
    else{
        for (uint8_t i = 0; i<(MCU.bloc_col*MCU.bloc_line); i++){
            free_MCU(*(MCU.blocs[i]));
            free(MCU.blocs[i]);
        }
    }
    free(MCU.blocs);
    //free(&MCU);
}


void free_MCU_YCbCr(struct MCU_YCbCr MCU)
{   
    for (uint8_t i = 0; i<(MCU.col*MCU.lines); i++){
        if (MCU.blocs[i].Y != NULL) free_matrix(MCU.blocs[i].Y, 8);
        if (MCU.blocs[i].Cb != NULL) free_matrix(MCU.blocs[i].Cb, 8);
        if (MCU.blocs[i].Cr != NULL) free_matrix(MCU.blocs[i].Cr, 8);
    }
    free(MCU.blocs);
    //free(&MCU);
}


/*
    Permet de free une matrice des MCUs
*/

void free_img_MCU(struct matrix_MCU img)
{
    for (uint8_t i = 0; i<img.MCU_lines; i++){
        for (uint8_t j = 0; j<img.MCU_col; j++){
            free_MCU(img.matrice[i][j]);
            //free(&(img.matrice[i][j]));
        }
    }
    for (uint8_t j = 0; j<img.MCU_lines; j++) free(img.matrice[j]);
    free(img.matrice);
}


/*
    Cette fonction permet de créer une MCU en donnant comme parametre une matrice de taille quelconque 
    ainsi que le nombre de blocs en colonnes et en lignes qu'on veut 
*/

struct MCU_s *creer_MCU(int16_t **matrix_MCU, uint8_t bloc_col, uint8_t bloc_line)
{
    //Test du format de la MCU
    if (bloc_col > 2 || bloc_line > 2){
        printf("Format de MCU non supporté.\n");
        return NULL;
    }
    //Partition de la MCU
    struct MCU_s *MCU = malloc(sizeof(struct MCU_s));
    MCU->bloc_col = bloc_col;
    MCU->bloc_line = bloc_line;
    switch (bloc_col*bloc_line)
    {
    // Si la MCU est 2x2 blocs
    case 4:
        MCU->blocs = calloc(bloc_col*bloc_line, sizeof(struct MCU_s));
        int16_t **matrice1 = calloc(8, sizeof(int16_t *));
        int16_t **matrice2 = calloc(8, sizeof(int16_t *));
        int16_t **matrice3 = calloc(8, sizeof(int16_t *));
        int16_t **matrice4 = calloc(8, sizeof(int16_t *));
        for (uint8_t i = 0; i<8; i++){
            matrice1[i] = calloc(8, sizeof(int16_t));
            matrice2[i] = calloc(8, sizeof(int16_t));
            matrice3[i] = calloc(8, sizeof(int16_t));
            matrice4[i] = calloc(8, sizeof(int16_t));
            for (uint8_t j = 0; j<8; j++){
                matrice1[i][j] = matrix_MCU[i][j];
                matrice2[i][j] = matrix_MCU[i][j+8];
                matrice3[i][j] = matrix_MCU[i+8][j];
                matrice4[i][j] = matrix_MCU[i+8][j+8];
            }
        }
        MCU->blocs[0] = creer_MCU(matrice1, 1, 1);
        MCU->blocs[1] = creer_MCU(matrice2, 1, 1);
        MCU->blocs[2] = creer_MCU(matrice3, 1, 1);
        MCU->blocs[3] = creer_MCU(matrice4, 1, 1);
        break;
    // Si la MCU est 2x1 ou 1x2 blocs
    case 2:
        MCU->blocs = calloc(bloc_col*bloc_line, sizeof(struct MCU_s));
        if (bloc_line == 2){
            int16_t **matrice_haut = calloc(2*8, sizeof(int16_t *));
            int16_t **matrice_bas = calloc(2*8, sizeof(int16_t *));
            for (uint8_t i = 0; i<8; i++){
                matrice_haut[i] = calloc(8, sizeof(int16_t));
                matrice_bas[i] = calloc(8, sizeof(int16_t));
                for (uint8_t j = 0; j<8; j++){
                    matrice_haut[i][j] = matrix_MCU[i][j];
                    matrice_bas[i][j] = matrix_MCU[i+8][j];
                }
            }
            MCU->blocs[0] = creer_MCU(matrice_haut, 1, 1);
            MCU->blocs[1] = creer_MCU(matrice_bas, 1, 1);
        }
        else{
            int16_t **matrice_droite = calloc(8, sizeof(int16_t *));
            int16_t **matrice_gauche = calloc(8, sizeof(int16_t *));
            for (uint8_t i = 0; i<8; i++){
                matrice_droite[i] = calloc(2*8, sizeof(int16_t));
                matrice_gauche[i] = calloc(2*8, sizeof(int16_t));
                for (uint8_t j = 0; j<8; j++){
                    matrice_gauche[i][j] = matrix_MCU[i][j];
                    matrice_droite[i][j] = matrix_MCU[i][j+8];
                }
            }
            MCU->blocs[0] = creer_MCU(matrice_gauche, 1, 1);
            MCU->blocs[1] = creer_MCU(matrice_droite, 1, 1);
        }
        break;
    // Si la MCU est 1x1 bloc
    default:
        MCU->matrix = matrix_MCU;
        MCU->blocs = NULL;
        break;
    }
    return MCU;
}

/*
    Crée une liste dans l'ordre qu'on veut, du découpage des matrices qu'on veut tq la matrice qu'on découpe est de taille quelconque 
    et que les sous matrices qu'on retrouve sont de taille 8x8
*/

struct matrix *decoupe_matrice(struct matrix matrix_import, uint32_t MCU_col, uint32_t MCU_lines)
{
    //On crée un tableau dont chaque composante pointe vers un sous_tableau 8x8 ( une sous_matrice 8x8)
    struct matrix *sub_matrix = calloc(MCU_col*MCU_lines, sizeof(struct matrix)); // la liste des sous_matrices qu'on va retourner
    //Remplissage de chaque sous_matrice
    for (uint32_t k = 0; k<MCU_lines*MCU_col; k++){
        int16_t **matrice_tampon = calloc(8, sizeof(int16_t *)); // la matrice intermédiaire qu'on va remplire
        for (uint8_t i = 0; i<8; i++){
            uint32_t next_line = 8*((uint16_t) floor(((float) k)/((float) MCU_col)));
            matrice_tampon[i] = calloc(8, sizeof(int16_t));
            for (uint8_t j = 0; j<8; j++){
                uint32_t next_col = 8*(k%MCU_col);
                //Si on dépasse les indices de l'image importée, on étend le reste comme indiqué dans le sujet
                if (i+next_line >= matrix_import.lines || j+next_col >= matrix_import.col){
                    if (i+next_line >= matrix_import.lines) matrice_tampon[i][j] = matrix_import.matrice[matrix_import.lines-1][j];
                    else if (j+next_col >= matrix_import.col) matrice_tampon[i][j] = matrix_import.matrice[i][matrix_import.col-1];
                }
                else matrice_tampon[i][j] = matrix_import.matrice[i+next_line][j+next_col];
            }
        }
        //Insertion dans le tableau des sous matrices
        struct matrix struct_tampon = {matrice_tampon, 8, 8};
        sub_matrix[k] = struct_tampon;
        if (k == MCU_lines*MCU_col -1) free_matrix(matrice_tampon, 8);
    }
    return sub_matrix;
}

/*
    Cette fonction prend comme parametre une matrice initiale tres grande ( la matrice lu dans le fichier qu'on veur compresser)
    et nous rend une matrice des MCU découpé dans l'idéal selon le nombre qu'on veut de blocs (2x2, 2x1, 1x2, 1x1) mais dans un début, on fix le nombre de blocs
    pour chque MCU à 1x1 cad chauqe MCU est de taille 8x8
*/

struct matrix_MCU image_to_MCU(struct matrix matrix_import, uint32_t bloc_lines, uint32_t bloc_col)
{
    //MCU_col et MCU_lines sont respectivement le nombre de MCU's horizontalement et verticalement
    //à choisir pour réaliser le découpage de l'image
    uint32_t MCU_col = (int) ceilf(((float) matrix_import.col)/8);
    uint32_t MCU_lines = (int) ceilf(((float) matrix_import.lines)/8);
    //A MODIFIER SI TU VEUX DES MCUS DE TAILLE AUTRE QUE 8x8
    struct matrix *sub_matrix = decoupe_matrice(matrix_import, MCU_col, MCU_lines);
    struct MCU_s **matrix_of_MCU = calloc(MCU_lines, sizeof(struct MCU_s *));
    for (uint32_t i = 0; i<MCU_lines; i++){
        matrix_of_MCU[i] = calloc(MCU_col, sizeof(struct MCU_s));
        for (uint32_t j = 0; j<MCU_col; j++){
            struct matrix sub_mat = sub_matrix[j+MCU_col*i];
            if (sub_mat.col*sub_mat.lines != bloc_col*bloc_lines*64){
                printf("Le format du découpage n'est pas compatible avec la taille des MCU's demandée\n");
                exit(EXIT_FAILURE);
            }
            struct MCU_s *mcu_add = creer_MCU(sub_mat.matrice, bloc_col, bloc_lines);
            matrix_of_MCU[i][j] = *mcu_add;
            free(mcu_add);
        }
    }
    struct matrix_MCU matrix_of_MCU_struct = {matrix_of_MCU, MCU_col, MCU_lines};
    free(sub_matrix);
    return matrix_of_MCU_struct;
}





/*int main(void)
{
    int16_t pixel = {100};
    int16_t pixel1 = {200};
    int16_t pixel2 = {50};
    // int16_t **tab = calloc(17, sizeof(int16_t *));
    // for (uint32_t i = 0; i<17; i++){
    //     tab[i] = calloc(16, sizeof(int16_t));
    //     for (uint32_t j = 0; j<16; j++){
    //         if (j == 15) tab[i][j] = pixel2;
    //         else if (i == 16) tab[i][j] = pixel1;
    int16_t **tab = calloc(8, sizeof(int16_t *));
    for (int i = 0; i<8; i++){
        tab[i] = calloc(8, sizeof(int16_t));
        for (int j = 0; j<8; j++){
            if (j == 6) tab[i][j] = pixel2;
            else if (i == 7) tab[i][j] = pixel1;
            else tab[i][j] = pixel;
        }
    }
    //affiche_matrice(tab, 16, 17);
    struct matrix matrixx = {
        tab, 8, 8
    };
    //struct matrix *sub = decoupe_matrice(matrixx, 2, 3);
    //affiche_matrice(sub[5].matrice, 8, 8);
    //struct MCU_s *mcu = creer_MCU(tab, 2, 2);
    //affiche_matrice(mcu->blocs[0]->matrix, 8,8);
    struct matrix_MCU imgmc = image_to_MCU(matrixx, 1, 1);
    affiche_matrice(imgmc.matrice[0][0].matrix, 8, 8);
    free_img_MCU(imgmc);
    //free_MCU(*mcu);
    //free(mcu);
    // free_matrix(tab, 17);

    free_matrix(tab, 8);
}
*/