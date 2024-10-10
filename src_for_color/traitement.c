#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>
#include "assert.h"
#include "traitement.h"

/*  
    Traite les différents cas de la récupération des paramètres sur la ligne de commande:
        - Si donné sans arguments ou bien avec --help on retourne le fonctionnement du programme
        - --outfile= : permet de choisir le nom de la nouvelle image compréssé
        - --sample = : permet de redéfinir les facteurs d'échatillonnage choisi par défaut
*/

void traitement( int argc, char *argv[], char **nv_nom, char **chemin_image, uint8_t **facteurs_echantillonages){

    if (argc < 2){
        printf(" Utilisation : \n./ppm2jpg image.( ppm - pgm ). \n--outfile = new_name.jpeg. \n--sample = h1xv1,h2xv2,h3xv3. \n");
        printf("--outfile : permet de changer le nom de la nouvelle image compréssé. \n");
        printf("--sample : permet de redéfinir les facteurs d'échantillonages choisis par défaut.");
        exit(EXIT_FAILURE);
    } else {
        bool meme_nom = true;
        uint8_t elm;
        char * pas_nes = malloc(sizeof(char ));
        char * pas_nes_2 = malloc(sizeof(char ));
        assert(pas_nes_2 != NULL);
        for (uint8_t i = 1; i < argc ; i++){
            char * arg = argv[i];

            if ( strncmp("--outfile=", arg, 10) == 0){
                char * pas_nes = malloc(sizeof(char )*(strlen(arg) - 8));
                strncpy(pas_nes, arg + 10, strlen(arg) - 9);
                *nv_nom = pas_nes;
                meme_nom = false;
            } 

            else if ( strncmp( arg, ".", 1) == 0 || (strncmp( arg, "--", 2) != 0)   ) {
                *chemin_image = arg;
                if ( meme_nom ){
                    char * pas_nes = malloc(sizeof(char )*(strlen(arg) - 2));
                    printf(" ");
                    strncpy(pas_nes, arg, strlen(arg) - 3 );
                    *nv_nom = pas_nes;
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
            } else if ( strncmp(arg, "--help", 6) == 0){
                printf(" Utilisation : \n./ppm2jpg image.( ppm - pgm ). \n--outfile = new_name.jpeg. \n--sample = h1xv1,h2xv2,h3xv3. \n");
                printf("--outfile : permet de changer le nom de la nouvelle image compréssé. \n");
                printf(" --sample : permet de redéfinir les facteurs d'échantillonages choisis par défaut.");        
                exit(EXIT_FAILURE);
            }
        }
    }
}
