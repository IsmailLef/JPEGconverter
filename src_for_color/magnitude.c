#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

#include "magnitude.h"



// struct magnitude_s{
//      uint16_t magnitude;
//      uint16_t indice;
// };


/*
     Cette fontion prend comme parametre une valeur et retourne une structure magnitude relatif à cette valeur en donnant:
          - Sa magnitude
          - Son indice
          - La taille minimal sur laquelle on peut coder son indice en binaire
*/

struct magnitude_s get_magnitude(int16_t valeur)
{
     if (valeur > 2047 || valeur <-2047){
          printf("La valeur dépasse les valeurs encodées par le tableau de magnitude\n");
          exit(EXIT_FAILURE);
     }


     if (valeur == 0){
          struct magnitude_s mag = {0, 0};
          return mag;
     }

     uint16_t absolue = abs(valeur);
     uint16_t magnitude =1 ;
     uint16_t indice ;


     int16_t bornemax= pow(2,magnitude) - 1 ;
     int16_t bornemin = pow(2,magnitude-1) ; 

     //uint32_t exposant = pow(2,2);

     //printf(" l exposant  est : %d " , exposant );

     while (absolue > bornemax){
          magnitude ++ ; 
          bornemax= pow(2,magnitude) - 1 ;
          bornemin = pow(2,magnitude-1) ; 
     }

     if (valeur <= -(bornemin)){
          indice = valeur + (bornemax);
     }

     else if (valeur >= (bornemin)){
          indice =   (bornemax - bornemin +1) + valeur -bornemin  ;
     }


     struct magnitude_s mag = {magnitude, indice};


     
     return mag;

//      }
}


/*int main(void)
{
     printf("mag est %u et indice est %u\n", get_magnitude(0x0036).magnitude, get_magnitude(0x0036).indice);
}*/