
#include <stdlib.h> 
#include <stdint.h> 
#include <stdio.h> 
#include <string.h> 
#include <ctype.h> 
#include <math.h> 
#include "DCT.h"

#define PI 3.14159

float C(uint32_t i){
    if (i ==0){
        return (float)(1)/(sqrtf(2)) ;
    }      

    else{
        return ( (float)(1) ) ;
    }

}



float calc(int16_t** mat_pixel , uint32_t i , uint32_t j, uint32_t taille ){

    float valeur=0;

     for (uint32_t x =0 ; x < taille ; x++ ){
         for (uint32_t y =0 ; y < taille ; y++ ){
            
            valeur += ((float)(mat_pixel[x][y]))*(cos((float)((2*x +1)*i*PI)/(2*taille)))*(cos((float)((2*y +1)*j*PI)/(2*taille)));


         }
     } 

     return valeur ;

} 




int16_t** DCT( int16_t** mat_pixel , uint32_t taille ){

// retrancher 128
for(uint32_t i =0 ; i < taille ; i++){

    for(uint32_t j =0 ; j < taille ; j++){
        mat_pixel[i][j] -=128 ;
    }
} 


// creer la matrice de frequence 
    int16_t** mat_freq = calloc(taille , sizeof(int16_t*)) ;
    for (uint32_t i = 0 ; i < taille ; i++){ 
        mat_freq[i]=calloc(taille , sizeof(int16_t));
        for(uint32_t j =0 ; j < taille ; j++){
            mat_freq[i][j] = ((float)(2)/(taille))*(C(i))*(C(j))*(calc(mat_pixel,i,j,taille)) ;
        }
    }
    free_matrix(mat_pixel, taille);

return mat_freq;
}
