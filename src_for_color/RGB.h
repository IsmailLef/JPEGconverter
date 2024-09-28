#ifndef _RGB_H_
#define _RGB_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <time.h>
//#include "decoupe.h"
#include "decoupe_pgm.h"

// struct pixel_R{
//     // on a juste des valeurs positifs donc on travaille avec des unsigned
//     uint8_t R;
//     uint8_t G;
//     uint8_t B;
// };

// struct matrix {
//     int16_t **matrice;
//     int col;
//     int lines;
// };


// struct MCU_s {
//     uint8_t bloc_col;
//     uint8_t bloc_line;
//     struct MCU_s **blocs; // un pointeur vers les blocks de la marice
//     struct pixel_R **matrix; // NULL quand on a une matrice autre que 8x8, et contient la matrice 8x8  si c'est sa taille
// };

struct Matrix_YCbCr{
    int16_t **Y;
    int16_t  **Cb;
    int16_t **Cr;
};


struct pixel_Y{
    // on a fait des int car les valeurs peuvent être négatif
    int16_t Y;
    int16_t Cb;
    int16_t Cr;
};

struct MCU_YCbCr {
    struct Matrix_YCbCr *blocs;
    uint8_t col;
    uint8_t lines;
};

struct matrix_MCU_YCbCr{
    //Convertit la matrice des MCU_s correspondante à l'image en MCU_s YCbCr
    struct MCU_YCbCr **matrice;
    uint32_t bloc_col;
    uint32_t bloc_line;
};



extern struct pixel_Y *RGB_to_YCbCr( struct pixel_R *pixel );


extern struct Matrix_YCbCr *conversion( struct MCU_s * matrice_initiale);


extern struct MCU_YCbCr *MCU_to_YCbCr(struct MCU_s *MCU);


extern struct matrix_MCU_YCbCr *matrix_MCU_to_YCbCr(struct matrix_MCU matrix_MCU);

#endif  /* _RGB_H_ */