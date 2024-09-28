#ifndef _DECOUPE_H_
#define _DECOUPE_H_



#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>


struct matrix {
    struct pixel_R **matrice;
    uint32_t col;
    uint32_t lines;
};

struct matrix_MCU{
    struct MCU_s **matrice;
    uint32_t MCU_col;
    uint32_t MCU_lines;
};

struct pixel_R{
    uint8_t R;
    uint8_t G;
    uint8_t B;
};

struct MCU_s {
    uint8_t bloc_col;
    uint8_t bloc_line;
    struct MCU_s **blocs;
    struct pixel_R **matrix;
};


extern void affiche_matrice(struct pixel_R **matrixx, uint32_t col, uint32_t line);

extern void free_matrix(struct pixel_R **matrix, uint8_t lines);


extern void free_MCU(struct MCU_s MCU);


extern void free_img_MCU(struct matrix_MCU img);


extern struct MCU_s *creer_MCU(struct pixel_R **matrix_MCU, uint8_t bloc_col, uint8_t bloc_line);


extern struct matrix *decoupe_matrice(struct matrix matrix_import, uint32_t MCU_col, uint32_t MCU_lines);


extern struct matrix_MCU image_to_MCU(struct matrix matrix_import, uint32_t bloc_lines, uint32_t bloc_col);


#endif  /* _PPM_FILE_H_ */