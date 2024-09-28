#ifndef _RLE_H_
#define _RLE_H_


#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <string.h>
#include "RGB.h"
#include "magnitude.h"
#include "bitstream.h"
#include "huffman.h"
//#include "huffman_refaite.h"


// struct pixel_Y{
//     // on a fait des int car les valeurs peuvent être négatif
//     int16_t Y;
//     int16_t Cb;
//     int16_t Cr;
// };



// struct magnitude_s{
//      uint16_t magnitude;
//      uint16_t indice;
// };


// struct Matrix_YCbCr{
//     int16_t **Y;
//     int16_t  **Cb;
//     int16_t **Cr;
// };

// struct MCU_YCbCr {
//     struct Matrix_YCbCr *blocs;
//     int col;
//     int lines;
// };


struct vector{
    int16_t **vector;
    uint8_t byte_lenght;
};



struct MCU_vector{
    struct vector vector_Y;
    struct vector vector_Cb;
    struct vector vector_Cr;
};



// struct RLE_to_bitstream{
//     uint16_t first_byte;
//     uint16_t second_byte;
//     uint16_t index;
//     uint8_t *nb_bits;
// };


// struct matrix {
//     int16_t **matrice;
//     int col;
//     int lines;
// };


struct DC_MCU{
    int16_t *Y;
    int16_t *Cb;
    int16_t *Cr;
};

extern void free_vector(struct MCU_vector *vect);

extern uint8_t count_neqz_coeff(int16_t *vector);


extern uint8_t concatenate_hex(uint8_t first_byte ,uint8_t second_byte);


extern void *code_AC_RLE(int16_t *vector, struct bitstream *bitstream, struct huff_table *hf);


extern struct MCU_vector *MCU_to_vector(struct MCU_YCbCr *MCU);

extern struct DC_MCU *code_DC(struct MCU_vector *MCU_vect);

extern void free_DC_vect(struct DC_MCU *vect_DC);

#endif  /* _RLE_H_ */

