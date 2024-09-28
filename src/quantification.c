#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#include "quantification.h"


/*
    Fait la quantification de la composante Y en donnant son vecteur
*/

void *quantificate_Y(int16_t *vector)
{
    for (uint8_t i = 0; i<64; i++){
        vector[i] = (int16_t) roundf(((float) vector[i])/((float) quantification_table_Y[i]));
    }
    return EXIT_SUCCESS;
}


/*
    Fait la quantification de la composante Cb ou Cr en donnant son vecteur
*/


void *quantificate_C(int16_t *vector)
{
    for (uint8_t i = 0; i<64; i++){
        vector[i] = (int16_t) roundf(((float) vector[i])/((float) quantification_table_CbCr[i]));
    }
    return EXIT_SUCCESS;
}

/*void main(void)
{
    int16_t *vect = calloc(64, sizeof(int16_t));
    for (uint8_t i = 0; i<64; i++) vect[i] = i;
    quantificate_Y(vect);
    for (uint8_t i = 0; i<64; i++) printf("%x ",vect[i]);
}*/