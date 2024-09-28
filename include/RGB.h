#ifndef _RGB_H_
#define _RGB_H_

#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <time.h>


struct pixel_R{
    // on a juste des valeurs positifs donc on travaille avec des unsigned
    uint8_t R;
    uint8_t G;
    uint8_t B;
};

struct matrix {
    int8_t **matrice;
    int col;
    int lines;
};


struct MCU_s {
    uint8_t bloc_col;
    uint8_t bloc_line;
    struct MCU_s **blocs; // un pointeur vers les blocks de la marice
    struct pixel_R **matrix; // NULL quand on a une matrice autre que 8x8, et contient la matrice 8x8  si c'est sa taille
};

struct Matrix_YCbCr{
    uint8_t **Y;
    uint8_t  **Cb;
    uint8_t **Cr;
};


struct pixel_Y{
    // on a fait des int car les valeurs peuvent être négatif
    uint8_t Y;
    uint8_t Cb;
    uint8_t Cr;
};




#endif  /* _RGB_H_ */