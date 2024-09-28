#ifndef _MAGNITUDE_H_
#define _MAGNITUDE_H_

#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>


struct magnitude_s{
     uint16_t magnitude;
     uint16_t indice;
};


extern struct magnitude_s get_magnitude(int16_t valeur);


#endif  /* _MAGNITUDE_H_ */