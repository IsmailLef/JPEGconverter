#ifndef _PGM_FILE_H_
#define _PGM_FILE_H_



#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>


extern int16_t ** PGM_FILE (char * file_name, uint32_t *colonne_mat, uint32_t *ligne_mat);

#endif  /* _PPM_FILE_H_ */