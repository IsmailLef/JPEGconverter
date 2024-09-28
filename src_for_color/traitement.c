#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "traitement.h"

/*  
    Traite les différents cas de la récupération des paramètres sur la ligne de commande:
        - Si donné sans arguments ou bien avec --help on retourne le fonctionnement du programme
        - --outfile= : permet de choisir le nom de la nouvelle image compréssé
        - --sample = : permet de redéfinir les facteurs d'échatillonnage choisi par défaut
*/

void traitement( int argc, char *argv[], char **nv_nom, char **chemin_image, uint8_t **facteurs_echantillonages){


    if (argc < 2){
        printf(" Utilisation : ./ppm2jpg image.( ppm - pgm ) --outfile = new_name.jpeg  --sample = h1xv1,h2xv2,h3xv3 \n");
        printf("--outfile : permet de changer le nom de la nouvelle image compréssé \n");
        printf(" --sample : permet de redéfinir les facteurs d'échantillonages choisis par défaut. \n ");
    }else{
        // char * nv_nom;
        // char * chemin_image;
        // uint8_t facteurs_echatillonages [6];
        // uint8_t longeur_arg = strlen(argv[1]);
        bool meme_nom = true;
        // *nv_nom = 
        // *nv_nom = strncpy(*nv_nom, argv[1], longeur_arg - 3 );
        uint8_t elm;
        char * pas_nes = malloc(sizeof(char *));
        char * pas_nes_2 = malloc(sizeof(char *));
        for (uint8_t i = 1; i < argc ; i++){
            char * arg = argv[i];

            if ( strncmp("--outfile=", arg, 10) == 0){    
                *nv_nom = strncpy(pas_nes_2, arg + 10, strlen(arg) - 9);
                meme_nom = false;
            } 

            else if ( strncmp( arg, ".", 1) == 0 || (strncmp( arg, "--", 2) != 0)   ) {
                *chemin_image = arg;
                if ( meme_nom ){
                    printf(" ");
                    // printf("On garde le même nom\n");
                    *nv_nom =  strncpy(pas_nes_2, arg, strlen(arg) - 3 );
                    // printf("le nom est : %s \n",*nv_nom);
                    strcat(*nv_nom, "jpg");
                }
            }else if ( strncmp("--sample=", arg, 9) == 0 ){
                
                pas_nes = strncpy(pas_nes, &arg[9], 1);
                elm = (uint8_t) atoi(pas_nes);
                (*facteurs_echantillonages)[0] =elm;

                pas_nes = strncpy(pas_nes, &arg[11], 1);
                elm = (uint8_t) atoi(pas_nes);
                (*facteurs_echantillonages)[1] = elm;

                

                pas_nes = strncpy(pas_nes, &arg[13], 1);
                elm = (uint8_t) atoi(pas_nes);
                (*facteurs_echantillonages)[2] = elm;

                pas_nes = strncpy(pas_nes, &arg[15], 1);
                elm = (uint8_t) atoi(pas_nes);
                (*facteurs_echantillonages)[3] = elm;

                
                pas_nes = strncpy(pas_nes, &arg[17], 1);
                elm = (uint8_t) atoi(pas_nes);
                (*facteurs_echantillonages)[4] = elm;

                
                pas_nes = strncpy(pas_nes, &arg[19], 1);
                elm = (uint8_t) atoi(pas_nes);
                (*facteurs_echantillonages)[5] = elm;




            }else if ( strncmp(arg, "--help", 6) == 0){
                printf(" Utilisation : ./ppm2jpg image.( ppm - pgm ) --outfile = new_name.jpeg  --sample = h1xv1,h2xv2,h3xv3 \n");
                printf("--outfile : permet de changer le nom de la nouvelle image compréssé \n");
                printf(" --sample : permet de redéfinir les facteurs d'échantillonages choisis par défaut. \n ");            
            }
        }
    }
}


/*int main(int argc, char ** argv){

    char ** nv_nom = malloc(sizeof(char *));
    printf("malloc a \n");
    char ** chemin_image = malloc(sizeof(char *));
    printf("malloc b \n");
    uint8_t **facteurs_echantillonages = malloc(sizeof(uint8_t *));
    printf("malloc c \n"); 
    *facteurs_echantillonages = calloc(100, sizeof(uint8_t));
    traitement(argc, argv, nv_nom, chemin_image, facteurs_echantillonages);
    printf("le nouveau nom de l'image généré est : %s\n", *nv_nom);
    printf("le chemin de l'image est : %s\n", *chemin_image);
    for (uint16_t i = 0; i < 6; i++){
        printf(" L'étape %i\n", i+1);
        printf("le %i ème élément est : %i\n", i, (*facteurs_echantillonages)[i]);
    }
}*/
