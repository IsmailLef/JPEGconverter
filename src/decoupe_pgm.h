#ifndef _DECOUPE_H_
#define _DECOUPE_H_



#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>


struct matrix {
    int16_t **matrice;
    uint32_t col;
    uint32_t lines;
};


struct matrix_MCU{
    struct MCU_s **matrice;
    uint32_t MCU_col;
    uint32_t MCU_lines;
};



struct MCU_s {
    int16_t bloc_col;
    int16_t bloc_line;
    struct MCU_s **blocs;
    int16_t **matrix;
};


struct pixel_R{
    int16_t R;
    int16_t G;
    int16_t B;
};

extern void affiche_matrice(int16_t **matrixx, uint32_t col, uint32_t line);

extern void free_matrix(int16_t **matrix, uint8_t lines);


extern void free_MCU(struct MCU_s MCU);


extern void free_img_MCU(struct matrix_MCU img);


extern struct MCU_s *creer_MCU(int16_t **matrix_MCU, uint8_t bloc_col, uint8_t bloc_line);


extern struct matrix *decoupe_matrice(struct matrix matrix_import, uint32_t MCU_col, uint32_t MCU_lines);


extern struct matrix_MCU image_to_MCU(struct matrix matrix_import, uint32_t bloc_lines, uint32_t bloc_col);


#endif  /* _PPM_FILE_H_ */