#ifndef _PPM_FILE_H_
#define _PPM_FILE_H_


#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "decoupe_pgm.h"


extern struct pixel_R *** donnes_brut (char * file_name, uint32_t *colonne_mat, uint32_t *ligne_mat);

#endif  /* _PPM_FILE_H_ */
