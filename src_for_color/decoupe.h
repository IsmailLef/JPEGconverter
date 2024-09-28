#ifndef _DECOUPE_H_
#define _DECOUPE_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>



// struct pixel_R{
//     int16_t R;
//     int16_t G;
//     int16_t B;
// };

struct pixel_R{
    uint8_t R;
    uint8_t G;
    uint8_t B;
};

struct matrix_C { // changement
    struct pixel_R ***matrice;
    uint32_t col;
    uint32_t lines;
};

struct MCU_s_C { // changement
    uint8_t bloc_col;
    uint8_t bloc_line;
    struct MCU_s_C **blocs; // changement
    struct pixel_R ***matrix;
};


struct matrix_MCU_C{ // changement
    struct MCU_s_C **matrice;  // changement
    uint32_t MCU_col;
    uint32_t MCU_lines;
};




extern void affiche_matrice(int16_t **matrixx, uint32_t col, uint32_t line);

/*
    La difference entre le decoupe et decoupe RGB est l'ajoute de "COuleur" à cote des noms des fonctions
    Et _C à cote des noms des structures

*/

extern void affiche_matrice_Couleur(struct pixel_R ***matrixx, uint32_t col, uint32_t line);


extern void free_matrix_Couleur(struct pixel_R ***matrix, uint32_t lines, uint32_t col);


extern void free_MCU_Couleur(struct MCU_s_C MCU);


extern void free_img_MCU_Couleur(struct matrix_MCU_C img);


extern struct MCU_s_C *creer_MCU_Couleur(struct pixel_R ***matrix_MCU, uint8_t bloc_col, uint8_t bloc_line);


extern struct matrix_C *decoupe_matrice_Couleur(struct matrix_C matrix_import, uint32_t MCU_col, uint32_t MCU_lines, uint32_t bloc_MCU_col, uint32_t bloc_MCU_lines);


extern struct matrix_MCU_C image_to_MCU_Couleur(struct matrix_C matrix_import, uint32_t bloc_lines, uint32_t bloc_col);


#endif  /* _DECOUPE_H_ */