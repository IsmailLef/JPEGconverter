#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <math.h>
#include "PPM_FILE.h"

#include "decoupe.h"
// struct pixel_R{
//     uint8_t R;
//     uint8_t G;
//     uint8_t B;
// };

// struct pixel_R{
//     int16_t R;
//     int16_t G;
//     int16_t B;
// };


// struct pixel_R *** main(int argc, char **argv){
struct pixel_R ***donnes_brut(char * file_name, uint32_t *colonne_mat, uint32_t *ligne_mat){

    FILE *file_r;
    // file_r=fopen( argv[1],"r");
    file_r=fopen( file_name,"r");

    char *ligne_ppm = malloc(sizeof(char)*255);



/* const char * separators =" ";
    uint8_t verif =0 ; // prends la valeur 0 si l en tete du fichier ne correspond pas P6

  
  //voir la premiere ligne si il s' agit d un  P6
   fgets(ligne_ppm  , 255 ,file_r);
   char *ligne1 =strtok(ligne  , separators); //Premiere donnée( P6 en principe)
   
   if ( strncmp( ligne1 , "P6" , strlen(ligne1)-1 ) == 0){
       verif =1  ;
   }
    
    if(verif==1 ){ 
        fgets(ligne_ppm , 255 ,file_r); 
        ....
        //supprimer le for et le const char separator ... 
        // fermer le if a avant le fclose tout en bas 

        // }
*/




    //creer la matrice par la deuxieme ligne 
    for (uint32_t i = 0 ; i < 2 ; i++){

        fgets(ligne_ppm , 255 ,file_r);
    }


    const char * separators =" ";
    char *ligne_colonne =strtok(ligne_ppm , separators); //Premiere donnée
    *colonne_mat= (uint32_t) atoi(ligne_colonne);   // nbr de colonne de l'image
    // printf("nb_colonnes : %i \n", *colonne_mat);
    ligne_colonne = strtok ( NULL, separators );// deuxieme donnée 
    *ligne_mat= (uint32_t) atoi(ligne_colonne); // nombre de ligne de l'image
    // printf("nb_ligne : %i \n", *ligne_mat);


    fgets(ligne_ppm , 255 ,file_r); // sauter la troisieme ligne


    //Definir la matrice 

    struct pixel_R ***matrice=calloc(*ligne_mat , sizeof(struct pixel_R **)) ;

    for (uint32_t i = 0 ; (i < *ligne_mat) && (!feof(file_r)); i++){ 
        matrice[i]=calloc(*colonne_mat , sizeof(struct pixel_R *));
        for (uint32_t j = 0; j < *colonne_mat; j++){
            struct pixel_R *pix =malloc(sizeof(struct pixel_R ));
            uint8_t mot =fgetc(file_r);
            pix->R =   mot  ; 
            mot =fgetc(file_r);
            pix->G =   mot  ;
            mot =fgetc(file_r);
            pix->B =  mot ; 
            matrice[i][j] = pix ;
        }
    } 
    fclose(file_r);
    free(ligne_ppm);
    return (matrice);

}



