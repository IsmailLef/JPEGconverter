#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>




//------------------------------------------------------------------------------------------------------------- PGM_FILE //
int16_t ** PGM_FILE(char * file_name, uint32_t *colonne_mat, uint32_t *ligne_mat){


FILE *file_r;
file_r=fopen( file_name,"r");

char *ligne_pgm = malloc(sizeof(char)*255);

//creer la matrice par la deuxieme ligne 

// d abord aller sur la deuxieme ligne 
for (uint32_t i = 0 ; i < 2 ; i++){
    fgets(ligne_pgm , 255 ,file_r);
}


const char * separators =" ";
char *ligne_colonne =strtok(ligne_pgm , separators); //Premiere donnée
*colonne_mat=atoi(ligne_colonne); // colonne matrice
ligne_colonne = strtok ( NULL, separators );// deuxieme donnée 
*ligne_mat=atoi(ligne_colonne); // ligne matrice



fgets(ligne_pgm , 255 ,file_r); // sauter la troisieme ligne

//Definir la matrice 

int16_t**matrice=calloc(*ligne_mat , sizeof(int16_t*)) ;
for (uint32_t i = 0 ; i < *ligne_mat ; i++){ 
    matrice[i]=calloc(*colonne_mat , sizeof(int16_t));
    for (uint32_t j = 0 ; j < *colonne_mat ; j++){ 
        uint8_t mot =fgetc(file_r);
        matrice[i][j] = mot ;

    }
} 

fclose(file_r);
free(ligne_pgm);
return matrice; 

} 

/*void main(int argc, char *argv[] )
{
    uint32_t *nb_col = malloc(sizeof(uint32_t));
    uint32_t *nb_line = malloc(sizeof(uint32_t));
    int16_t **matrice = PGM_FILE(argv[1], nb_col, nb_line);
}*/
