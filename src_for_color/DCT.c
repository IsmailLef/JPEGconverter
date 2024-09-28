
#include <stdlib.h> 
#include <stdint.h> 
#include <stdio.h> 
#include <string.h> 
#include <ctype.h> 
#include <math.h>



#define val 0.707106  // (1/racine(2))



float C(uint32_t i){
    if (i ==0){
        return (val);
    }      

    else{
        return ( (float)(1) ) ;
    }

} 




float calc(int16_t** mat_pixel , uint32_t i , uint32_t j, uint32_t taille, float **tab_cos){  // supprimer la taille car c est toujours 8 
 

//calcule de la valeur 

    float valeur=0;

     for (uint32_t x =0 ; x < taille ; x++ ){
         for (uint32_t y =0 ; y < taille ; y++ ){
            
            valeur += ((float)(mat_pixel[x][y]))*(tab_cos[i][x])*(tab_cos[j][y]);


         }
     } 

     return valeur ;

} 




int16_t** DCT( int16_t** mat_pixel , uint32_t taille, float **tab_cos){


// retrancher 128
    for(uint32_t k =0 ; k < taille ; k++){
        for(uint32_t l = 0 ; l < taille ; l++){
            mat_pixel[k][l] -=128;
        }
    } 


// creer la matrice de frequence 
    int16_t** mat_freq = calloc(taille , sizeof(int16_t*)) ;
    for (uint32_t a = 0 ; a < taille ; a++){ 
        mat_freq[a]=calloc(taille , sizeof(int16_t));
        for(uint32_t b = 0 ; b < taille ; b++){
            mat_freq[a][b] = ((float)(2)/(taille))*(C(a))*(C(b))*(calc(mat_pixel, a, b, taille, tab_cos));
        }
    }

    return mat_freq; 
}
