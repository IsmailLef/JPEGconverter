#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "decoupe_pgm.h"

// struct matrix {
//     int16_t **matrice;
//     int col;
//     int lines;
// };



int16_t *zigzag(struct matrix matrix_import)
{
    int16_t *vector = calloc(64, sizeof(int16_t));
    if (matrix_import.col != 8 || matrix_import.lines != 8){
        printf("L'opération zigzag se fait seulement sur des blocs 8x8. Revoyez la taille de votre entrée.");
        exit(EXIT_FAILURE);
    }
    vector[0] = matrix_import.matrice[0][0];
    uint16_t i = 0;
    uint16_t j = 0;
    uint16_t pos = 1;
    //Traversée de la triangulaire supérieure (à gauche)
    do {
        if (i == 0){
            j++;
            while (j != 0){
                vector[pos] = matrix_import.matrice[i][j];
                pos ++;
                j--;
                i++;
            }
            vector[pos] = matrix_import.matrice[i][j];
            pos ++;
        }
        else if (j == 0)
        {
            i++;
            while (i!=0){
                vector[pos] = matrix_import.matrice[i][j];
                pos ++;
                i--;
                j++;
            }
            vector[pos] = matrix_import.matrice[i][j];
            pos ++;
        }
    } while (j != 6);
    //Là on se trouve sur la diagonale opposée de la matrice
    //on rajoute 1 à j pour faire la transition entre les deux triangulaires
    //Ensuite on reboucle sur la deuxième triangulaire
    do {
        if (i == 7){
            j++;
            while (j != 7){
                vector[pos] = matrix_import.matrice[i][j];
                pos ++;
                //printf("(i,j) = (%u,%u)\n", i, j);
                j++;
                i--;
            }
            vector[pos] = matrix_import.matrice[i][j];
            pos ++;
            //printf("(i,j) = (%u,%u)\n", i, j);
        }
        else if (j == 7){
            i++;
            while (i != 7){
                vector[pos] = matrix_import.matrice[i][j];
                pos ++;
                //printf("(i,j) = (%u,%u)\n", i, j);
                i++;
                j--;
            }
            vector[pos] = matrix_import.matrice[i][j];
            pos ++;
            //printf("(i,j) = (%u,%u)\n", i, j);
        }
        else {
            j++;
            vector[pos] = matrix_import.matrice[i][j];
            pos ++;
            //printf("(i,j) = (%u,%u)\n", i, j);
            while (i != 7){
                j--;
                i++;
                vector[pos] = matrix_import.matrice[i][j];
                pos ++;
                //printf("(i,j) = (%u,%u)\n", i, j);
            }
        }
    } while (pos != 64);
    return vector;
}




/*void main(void)
{
    int16_t **tab = calloc(8, sizeof(int16_t *));
    for (int i = 0; i<8; i++){
        tab[i] = calloc(8, sizeof(int16_t));
        for (int j = 0; j<8; j++){
            tab[i][j] = i+j;
        }
    }
    struct matrix matrixx = {
        tab, 8, 8
    };
    //affiche_matrice(tab, 8, 8);
    int16_t *vect = zigzag(matrixx);
    for (int k =0; k<64; k++) printf("%d ", vect[k]);
    printf(" \n");
}*/