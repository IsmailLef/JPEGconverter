#ifndef _DCT_H_
#define _DCT_H_

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>


extern float C(uint32_t i);


extern float calc(int16_t ** mat_pixel , uint32_t i , uint32_t j, uint32_t taille, float **tab_cos);


extern int16_t** DCT( int16_t** mat_pixel , uint32_t taille, float **tab_cos);

#endif  /* _DCT_H_ */