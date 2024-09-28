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

/* const char * separators =" ";
    uint8_t verif =0 ;  // prends la valeur 0 si l en tete du fichier ne correspond pas P5

  
  //voir la premiere ligne si il s' agit d un  P5 
   fgets(ligne_pgm  , 255 ,file_r);
   char *ligne1 =strtok(ligne_pgm  , separators); //Premiere donnée( P5 en principe)
   
   if ( strncmp( ligne1 , "P5" , strlen(ligne1)-1 ) == 0){
       verif =1 ;
   }
    
    if(verif==1 ){
        fgets(ligne_ppm , 255 ,file_r); 
        ....
        //supprimer le for et le const char separator ... 
        // fermer le if a avant le fclose tout en bas 

        // }
*/






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
}

//remplissage de la matrice
uint32_t i_ligne =0 ;
uint32_t i_colonne=0;



while((!feof(file_r)) &&( i_ligne < *ligne_mat)){
//while(( i_ligne < *ligne_mat)){

     // Creation du pixel
  // struct pixel *pix =malloc(sizeof(struct pixel ));

 

uint8_t mot =fgetc(file_r);
      /*while(mot == 32 ){
         mot =fgetc(file_r);
     }; */

     //pix->val = mot ; 



matrice[i_ligne][i_colonne] = mot ;


// incrementer la colonne et passer à la ligne suivante si la colonne depasse
i_colonne++ ;
if (i_colonne >= *colonne_mat){
    i_colonne=0;
    i_ligne ++ ; 
    }

} //fin du while 


fclose(file_r);

// for ( uint32_t i = 0; i< *ligne_mat ; i++){
// for ( uint32_t j = 0; j< *colonne_mat; j++){

        
//       //  printf("[%02hx]  \t" , matrice[i][j] );
    
    
//     }
//     //printf("\n");
// } 


return ( matrice ) ; 

} 

/*void main(int argc, char *argv[] )
{
    uint32_t *nb_col = malloc(sizeof(uint32_t));
    uint32_t *nb_line = malloc(sizeof(uint32_t));
    int16_t **matrice = PGM_FILE(argv[1], nb_col, nb_line);
}*/
