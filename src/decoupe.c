#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "decoupe.h"


//  ########## COM DE MOURAD
// J'AI CHANGÉ TOUS LES INT EN UINT32_T CAR J'AI EU LA FLEMME DE REVOIR EXACTEMENT OU IL FAUT UINT8_T OU IL FAUT INT ET OU IL FAUT UINT8_T



// struct matrix {
//     struct pixel_R **matrice;
//     uint32_t col;
//     uint32_t lines;
// };

// struct matrix_MCU{
//     struct MCU_s **matrice;
//     uint32_t MCU_col;
//     uint32_t MCU_lines;
// };

// struct pixel_R{
//     uint8_t R;
//     uint8_t G;
//     uint8_t B;
// };

// struct MCU_s {
//     uint8_t bloc_col;
//     uint8_t bloc_line;
//     struct MCU_s **blocs;
//     struct pixel_R **matrix;
// };


void affiche_matrice_Couleur(struct pixel_R **matrixx, uint32_t col, uint32_t line) // validé normalement // changement
{
    for (uint32_t i = 0; i<line; i++){
        for (uint32_t j = 0; j<col; j++) printf("[ %x %x %x ] \t ", matrixx[i][j].R,matrixx[i][j].G,matrixx[i][j].B);
        printf(" \n");
    }
}


void free_matrix_Couleur(struct pixel_R **matrix, uint8_t lines) // validé normalement // changement
{
    for (uint8_t i = 0; i<lines; i++){
        free(matrix[i]);
    }
    free(matrix);
}




void free_MCU_Couleur(struct MCU_s_C MCU) // validé normalement // changement// changement
{   
    if (MCU.blocs == NULL){
        free_matrix_Couleur(MCU.matrix, 8); // changement
    }
    else{
        for (uint8_t i = 0; i<(MCU.bloc_col*MCU.bloc_line); i++){
            free_MCU_Couleur(*(MCU.blocs[i])); // changement
            free(MCU.blocs[i]);
        }
    }
    free(MCU.blocs);
    //free(&MCU);
}


void free_img_MCU_Couleur(struct matrix_MCU_C img)  // validé normalement // changement // changement
{
    for (uint8_t i = 0; i<img.MCU_lines; i++){
        for (uint8_t j = 0; j<img.MCU_col; j++){
            free_MCU_Couleur(img.matrice[i][j]); // changement
            //free(&(img.matrice[i][j]));
        }
    }
    for (uint8_t j = 0; j<img.MCU_lines; j++) free(img.matrice[j]);
    free(img.matrice);
}



//Requiert une taille de matrix_MCU aux bonnes dimensions et renvoie la MCU à partir de cette dernière
struct MCU_s_C *creer_MCU_Couleur(struct pixel_R **matrix_MCU, uint8_t bloc_col, uint8_t bloc_line) // validé normalement  // changement //changement
{
    //Test du format de la MCU
    if (bloc_col > 2 || bloc_line > 2){
        printf("Format de MCU non supporté.\n");
        return NULL;
    }
    //Partition de la MCU
    struct MCU_s_C *MCU = malloc(sizeof(struct MCU_s_C)); // changement // changement
    MCU->bloc_col = bloc_col;
    MCU->bloc_line = bloc_line;
    switch (bloc_col*bloc_line)
    {
    // Si la MCU est 2x2 blocs
    case 4:
        MCU->blocs = calloc(bloc_col*bloc_line, sizeof(struct MCU_s_C)); // changement
        struct pixel_R **matrice1 = calloc(8, sizeof(struct pixel_R *));
        struct pixel_R **matrice2 = calloc(8, sizeof(struct pixel_R *));
        struct pixel_R **matrice3 = calloc(8, sizeof(struct pixel_R *));
        struct pixel_R **matrice4 = calloc(8, sizeof(struct pixel_R *));
        for (uint8_t i = 0; i<8; i++){
            matrice1[i] = calloc(8, sizeof(struct pixel_R));
            matrice2[i] = calloc(8, sizeof(struct pixel_R));
            matrice3[i] = calloc(8, sizeof(struct pixel_R));
            matrice4[i] = calloc(8, sizeof(struct pixel_R));
            for (uint8_t j = 0; j<8; j++){
                matrice1[i][j] = matrix_MCU[i][j];
                matrice2[i][j] = matrix_MCU[i][j+8];
                matrice3[i][j] = matrix_MCU[i+8][j];
                matrice4[i][j] = matrix_MCU[i+8][j+8];
            }
        }
        MCU->blocs[0] = creer_MCU_Couleur(matrice1, 1, 1); // changement
        MCU->blocs[1] = creer_MCU_Couleur(matrice2, 1, 1); // changement
        MCU->blocs[2] = creer_MCU_Couleur(matrice3, 1, 1); // changement
        MCU->blocs[3] = creer_MCU_Couleur(matrice4, 1, 1); // changement
        break;
    // Si la MCU est 2x1 ou 1x2 blocs
    case 2:
        MCU->blocs = calloc(bloc_col*bloc_line, sizeof(struct MCU_s_C)); // changement
        if (bloc_line == 2){
            struct pixel_R **matrice_haut = calloc(2*8, sizeof(struct pixel_R *));
            struct pixel_R **matrice_bas = calloc(2*8, sizeof(struct pixel_R *));
            for (uint8_t i = 0; i<8; i++){
                matrice_haut[i] = calloc(8, sizeof(struct pixel_R));
                matrice_bas[i] = calloc(8, sizeof(struct pixel_R));
                for (uint8_t j = 0; j<8; j++){
                    matrice_haut[i][j] = matrix_MCU[i][j];
                    matrice_bas[i][j] = matrix_MCU[i+8][j];
                }
            }
            MCU->blocs[0] = creer_MCU_Couleur(matrice_haut, 1, 1); // changement
            MCU->blocs[1] = creer_MCU_Couleur(matrice_bas, 1, 1); // changement
        }
        else{
            struct pixel_R **matrice_droite = calloc(8, sizeof(struct pixel_R *));
            struct pixel_R **matrice_gauche = calloc(8, sizeof(struct pixel_R *));
            for (uint8_t i = 0; i<8; i++){
                matrice_droite[i] = calloc(2*8, sizeof(struct pixel_R));
                matrice_gauche[i] = calloc(2*8, sizeof(struct pixel_R));
                for (uint8_t j = 0; j<8; j++){
                    matrice_gauche[i][j] = matrix_MCU[i][j];
                    matrice_droite[i][j] = matrix_MCU[i][j+8];
                }
            }
            MCU->blocs[0] = creer_MCU_Couleur(matrice_gauche, 1, 1); // changement
            MCU->blocs[1] = creer_MCU_Couleur(matrice_droite, 1, 1); // changement
        }
    // Si la MCU est 1x1 bloc
    default:
        MCU->matrix = matrix_MCU;
        MCU->blocs = NULL;
        break;
    }
    return MCU;
}



struct matrix_C *decoupe_matrice_Couleur(struct matrix_C matrix_import, uint32_t MCU_col, uint32_t MCU_lines) // validé normalement // changement // changement // changement
{
    //On crée un tableau dont chaque composante pointe vers un sous_tableau 8x8
    printf("On alloue mnt");
    struct matrix_C *sub_matrix = calloc(MCU_col*MCU_lines, sizeof(struct matrix_C)); // changement // changement
    //Remplissage de chaque sous_matrice
    printf("Le nombre de MCUs de l'image est : ");
    for (uint8_t k = 0; k<MCU_lines*MCU_col; k++){
        struct pixel_R **matrice_tampon = calloc(8, sizeof(struct pixel_R *));
        for (uint8_t i = 0; i<8; i++){
            uint32_t next_line = 8*((uint8_t) floor(((float) k)/((float) MCU_col)));
            matrice_tampon[i] = calloc(8, sizeof(struct pixel_R));
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
        struct matrix_C struct_tampon = {matrice_tampon, 8, 8}; // changement
        sub_matrix[k] = struct_tampon;
    }
    //free_matrix(matrice_tampon, 8);
    return sub_matrix;
}

struct matrix_MCU_C image_to_MCU_Couleur(struct matrix_C matrix_import, uint32_t bloc_lines, uint32_t bloc_col) // validé normalement // changement // changement // changement
{
    //MCU_col et MCU_lines sont respectivement le nombre de MCU's horizontalement et verticalement
    //à choisir pour réaliser le découpage de l'image
    printf("On entre dans le découpages");
    uint32_t MCU_col = (int) ceilf(((float) matrix_import.col)/8);
    uint32_t MCU_lines = (int) ceilf(((float) matrix_import.lines)/8);
    //A MODIFIER SI TU VEUX DES MCUS DE TAILLE AUTRE QUE 8x8

    // marhce pas depuis cette ligne
    // printf("On est juste avant le découpage \n");
    struct matrix_C *sub_matrix = decoupe_matrice_Couleur(matrix_import, MCU_col, MCU_lines); // changement // changement

    printf("Le découpage marche !!\n");


    struct MCU_s_C **matrix_of_MCU = calloc(MCU_lines, sizeof(struct MCU_s_C *)); // changement // changement
    for (uint32_t i = 0; i<MCU_lines; i++){
        matrix_of_MCU[i] = calloc(MCU_col, sizeof(struct MCU_s_C)); // changement
        for (uint32_t j = 0; j<MCU_col; j++){
            struct matrix_C sub_mat = sub_matrix[j+MCU_col*i]; // changement
            if (sub_mat.col*sub_mat.lines != bloc_col*bloc_lines*64){
                printf("Le format du découpage n'est pas compatible avec la taille des MCU's demandée\n");
                exit(EXIT_FAILURE);
            }
            struct MCU_s_C *mcu_add = creer_MCU_Couleur(sub_mat.matrice, bloc_col, bloc_lines); // changement // changement
            matrix_of_MCU[i][j] = *mcu_add;
            free(mcu_add);
        }
    }
    struct matrix_MCU_C matrix_of_MCU_struct = {matrix_of_MCU, MCU_col, MCU_lines}; // changement
    //free(sub_matrix);
    return matrix_of_MCU_struct;
}





/*int main(void)
{
    struct pixel_R pixel = {100, 200, 50};
    struct pixel_R pixel1 = {200, 200, 50};
    struct pixel_R pixel2 = {50, 200, 50};
    // struct pixel_R **tab = calloc(17, sizeof(struct pixel_R *));
    // for (uint32_t i = 0; i<17; i++){
    //     tab[i] = calloc(16, sizeof(struct pixel_R));
    //     for (uint32_t j = 0; j<16; j++){
    //         if (j == 15) tab[i][j] = pixel2;
    //         else if (i == 16) tab[i][j] = pixel1;
    struct pixel_R **tab = calloc(16, sizeof(struct pixel_R *));
    for (int i = 0; i<16; i++){
        tab[i] = calloc(16, sizeof(struct pixel_R));
        for (int j = 0; j<16; j++){
            if (j == 15) tab[i][j] = pixel2;
            else if (i ==15) tab[i][j] = pixel1;
            else tab[i][j] = pixel;
        }
    }
    affiche_matrice(tab, 16, 16);
    // struct matrix matrixx = {
    //     tab, 8, 8
    // };
    //struct matrix *sub = decoupe_matrice(matrixx, 2, 3);
    //affiche_matrice(sub[5].matrice, 8, 8);
    struct MCU_s *mcu = creer_MCU(tab, 2, 2);
    affiche_matrice(mcu->blocs[1]->matrix, 8,8);
    // struct matrix_MCU imgmc = image_to_MCU(matrixx, 1, 1);
    // affiche_matrice(imgmc.matrice[0][0].matrix, 8, 8);
    // free_img_MCU(imgmc);
    //free_MCU(*mcu);
    //free(mcu);
    // free_matrix(tab, 17);

    free_matrix(tab, 8);
}*/
