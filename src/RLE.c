#include "traitement.h"
#include "PPM_FILE.h"
#include "decoupe_pgm.h"
#include "PGM_FILE.h"
#include "DCT.h"
#include "zigzag.h"
#include "quantification.h"
#include "magnitude.h"
#include "RLE.h"
#include "bitstream.h"
#include "huffman.h"
#include "htables.h"


#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include <string.h>


// struct pixel_Y{
//     // on a fait des int car les valeurs peuvent être négatif
//     int16_t Y;
//     int16_t Cb;
//     int16_t Cr;
// };



// struct magnitude_s{
//      uint16_t magnitude;
//      uint16_t indice;
// };


// struct Matrix_YCbCr{
//     int16_t **Y;
//     int16_t  **Cb;
//     int16_t **Cr;
// };

// struct MCU_YCbCr {
//     struct Matrix_YCbCr *blocs;
//     int col;
//     int lines;
// };


// struct vector{
//     int16_t **vector;
//     uint8_t byte_lenght;
// };



// struct MCU_vector{
//     struct vector vector_Y;
//     struct vector vector_Cb;
//     struct vector vector_Cr;
// };



// struct RLE_to_bitstream{
//     uint16_t first_byte;
//     uint16_t second_byte;
//     uint16_t index;
//     uint8_t *nb_bits;
// };


// struct matrix {
//     int16_t **matrice;
//     int col;
//     int lines;
// };


// struct DC_MCU{
//     int16_t *Y;
//     int16_t *Cb;
//     int16_t *Cr;
// };

void free_vector(struct MCU_vector *vect){
    if (vect->vector_Y.vector != NULL){
        for (uint8_t i = 0; i<vect->vector_Y.byte_lenght; i++){
            free(vect->vector_Y.vector[i]);
        }    
        free(vect->vector_Y.vector);
    }
    if (vect->vector_Cb.vector != NULL){
        for (uint8_t i = 0; i<vect->vector_Cb.byte_lenght; i++){
            free(vect->vector_Cb.vector[i]);
            }    
        free(vect->vector_Cb.vector);
    }
    if (vect->vector_Cr.vector != NULL){
        for (uint8_t i = 0; i<vect->vector_Cr.byte_lenght; i++){
            free(vect->vector_Cr.vector[i]);
        }    
        free(vect->vector_Cr.vector);
        }
    free(vect);
}

/*
    Le vecteur en entrée est forcément de taille 63, on fait du codage AC RLE ici.
    La valeur de retour est le nombre de coéfficients non nuls dans le vecteur en entrée
*/
uint8_t count_neqz_coeff(int16_t *vector)
{
    uint8_t counter = 0;
    for (uint8_t i = 1; i<63; i++){
        if (vector[i] != 0){
            counter++;
        }
    }
    return counter;
}



/*
    Concatène deux éléments de 8 octets
*/
uint8_t concatenate_hex(uint8_t first_byte ,uint8_t second_byte)
{
    return ((first_byte<<4)+(second_byte));
}




/*
    Le vecteur en entrée est forcément de taille 63, on fait du codage AC RLE ici.
    Produit un nouveau vecteur =< 63 codé façon RLE
*/
/*
    On fait le codage AC_RLE et on écrit directement apres dans le fichier à l'aide du bitstream
    Elle prend un vecteur et commence à coder ses élément suivant la méthode RLE à partir du 2ème elment ( le premier est DC)
    et envoir direcctement la valeur sur le bitstream
*/
void *code_AC_RLE(int16_t *vector, struct bitstream *bitstream, struct huff_table *hf)
{
    //Un compteur de zéros à l'instant t pour le premier octet
    uint16_t zero_occ = 0;
    //Pour se rappeler où on en est dans le vecteur en entrée
    uint32_t marqueur_vector = 1;
    //Allocation de mémoire pour le nouveau vecteur car inférieur à 63
    uint32_t non_zero_elm = count_neqz_coeff(vector);
    uint8_t first_byte;
    uint8_t second_byte;
    uint8_t *path_long_marker = malloc(sizeof(uint8_t));
    uint32_t eob = huffman_table_get_path(hf, 0x00, path_long_marker); // le code huffman à coder pour le end of block pour dire que la suite des AC est des 0
    uint8_t *path_long_marker2 = malloc(sizeof(uint8_t));
    uint32_t zrf = huffman_table_get_path(hf, 0xf0, path_long_marker2); // le code huffmane pour dire que les 16 valeurs suivantes sont en effet tous nuls
    uint8_t *path_long = malloc(sizeof(uint8_t));
    for (uint32_t i= 0; i< non_zero_elm; i++){
        zero_occ =  0;
        if (marqueur_vector == 63){     // si on atteint le dernier indice il faut sortir du parcours
            break;
        }
            while (vector[marqueur_vector] == 0 && marqueur_vector < 63){ // si l'element courant est null et qu'on a pas encore atteint le dernier élém ( d'où le < 63 pas < 64) on incrémente
            zero_occ++; // car vector[marqueur_vector] == 0 donc un nouveau élem null
            marqueur_vector++;  // car on avance dans le parcours
            if (zero_occ == 16){ // si on atteint 16 on peut plus le coder donc on écrite zrf et on rénitialise j'ai changé 15 en 16 car on peut quitter le while apres et donc coder 15
                zero_occ = 0;
                bitstream_write_bits(bitstream, zrf, *path_long_marker2, false);
            }
        }

        first_byte = zero_occ; // le premier byte est bien le  nombre de 0 donc zero_occ

        struct magnitude_s mag = get_magnitude(vector[marqueur_vector]); // on a besoin de la structure magnitude relatif à la valeur AC non null du vecteur 
        second_byte = mag.magnitude;    // le deuxième byte est la magnitude


        uint32_t value = huffman_table_get_path(hf, concatenate_hex(first_byte, second_byte), path_long); // on code la concatenation des 2 bytes
        bitstream_write_bits(bitstream, value, *path_long, false);
        bitstream_write_bits(bitstream, mag.indice, mag.magnitude, false);
        marqueur_vector++;
    }
    bitstream_write_bits(bitstream, eob, *path_long_marker, false); // on termine le bloc AC et donc on écrit le marqueur EOB
    free(path_long);
    free(path_long_marker2);
    free(path_long_marker);

    return EXIT_SUCCESS;
}




//En entrée une MCU et renvoie les vecteur associés aux Y, Cb et Cr à cette MCU après zigzag et quantification

/*
    Elle prend comme entrée une MCU contenant les composantes YCbCr et renvoie un vecteur qui contient une structure MCU_vector qui contient:
        - 3 vecteurs un pour Y, un autre pour Cb l'autre pour Cr
        - Chaque vecteur contient en effet au moins un vecteur et au plus 4 tq chacun représente la matrice de Y (relativement Cb et Cr) d'un bloc de la MCU
    Ces vecteurs sont en effet quantifié et zigzagé
*/
struct MCU_vector *MCU_to_vector(struct MCU_YCbCr *MCU)
{
    struct MCU_vector *vects = malloc(sizeof(struct MCU_vector));
    vects->vector_Y.vector = calloc(MCU->col*MCU->lines, sizeof(int16_t *));
    vects->vector_Cb.vector = calloc(MCU->col*MCU->lines, sizeof(int16_t *));
    vects->vector_Cr.vector = calloc(MCU->col*MCU->lines, sizeof(int16_t *));
    int16_t **vects_Y = vects->vector_Y.vector;
    int16_t **vects_Cb = vects->vector_Cb.vector;
    int16_t **vects_Cr = vects->vector_Cr.vector;
    for (uint32_t i = 0; i<MCU->col*MCU->lines; i++){
        vects->vector_Y.byte_lenght = 0;
        vects->vector_Cb.byte_lenght = 0;
        vects->vector_Cr.byte_lenght = 0;
        if (MCU->blocs[i].Y != NULL){
            struct matrix matrixxY = {MCU->blocs[i].Y, 8, 8};
            vects_Y[i] = zigzag(matrixxY);
            quantificate_Y(vects_Y[i]);
            vects->vector_Y.byte_lenght = i+1;
        }
        if (MCU->blocs[i].Cb != NULL){
            struct matrix matrixxCb = {MCU->blocs[i].Cb, 8, 8};
            vects_Cb[i] = zigzag(matrixxCb);
            quantificate_C(vects_Cb[i]);
            vects->vector_Cb.byte_lenght = i+1;
        }
        if (MCU->blocs[i].Cr != NULL){
            struct matrix matrixxCr = {MCU->blocs[i].Cr, 8, 8};
            vects_Cr[i] = zigzag(matrixxCr);
            quantificate_C(vects_Cr[i]);
            vects->vector_Cr.byte_lenght = i+1;
        }
    }
    return vects;
}



//Prend en entrée une structute vecteur zigzag d'une MCU complète et renvoie le codage des DC des blocs de la MCU respectivement dans un vecteur

/*
    Prend comme parametre une MCU_vector et retourne la structure DC_MCU qui pointe vers 3 élems ( Y Cb et Cr) contenant chacune une liste des valeurs de DC
    de chaque bloc de Y ( même cas pour Cb et Cr)
    Défaut : coder par rapport à la valeur qui la précede :) 
*/
struct DC_MCU *code_DC(struct MCU_vector *MCU_vect)
{
    uint32_t len_Y = MCU_vect->vector_Y.byte_lenght;
    uint32_t len_Cb = MCU_vect->vector_Cb.byte_lenght;
    uint32_t len_Cr = MCU_vect->vector_Cr.byte_lenght;
    struct DC_MCU *DC_vect = malloc(sizeof(struct DC_MCU));
    DC_vect->Y = calloc(len_Y, sizeof(int16_t));
    DC_vect->Cb = calloc(len_Cb, sizeof(int16_t));
    DC_vect->Cr = calloc(len_Cr, sizeof(int16_t));
    uint8_t flag_Y = 1;
    uint8_t flag_Cb = 1;
    uint8_t flag_Cr = 1;
    for (uint32_t i = 0; i<len_Y; i++){
        if (flag_Y == 1){
            DC_vect->Y[i] = MCU_vect->vector_Y.vector[i][0];
            flag_Y = 0;
        }
        else{
            DC_vect->Y[i] = MCU_vect->vector_Y.vector[i-1][0] - MCU_vect->vector_Y.vector[i][0];
        }      
    }
    for (uint32_t i = 0; i<len_Cb; i++){
        if (flag_Cb == 1){
            DC_vect->Cb[i] = MCU_vect->vector_Cb.vector[i][0];
            flag_Cb = 0;
        }
        else{
            DC_vect->Cb[i] = MCU_vect->vector_Cb.vector[i-1][0] - MCU_vect->vector_Cb.vector[i][0];
        }      
    }
    for (uint32_t i = 0; i<len_Cr; i++){
        if (flag_Cr == 1){
            DC_vect->Cr[i] = MCU_vect->vector_Cr.vector[i][0];
            flag_Cr = 0;
        }
        else{
            DC_vect->Cr[i] = MCU_vect->vector_Cr.vector[i-1][0] - MCU_vect->vector_Cr.vector[i][0];
        }      
    }
    return DC_vect;
}


void ecriture_MCU_ppm ( struct matrix_MCU_YCbCr matrice_MCUs, struct bitstream *notre_bitstream){

    struct Matrix_YCbCr **notre_matrice_YCbCr = matrice_MCUs.matrice;
    for (uint32_t i = 0; i < matrice_MCUs.bloc_line; i++){
        for (uint32_t j = 0; j < matrice_MCUs.bloc_col; j++){

            struct Matrix_YCbCr bloc_actu = notre_matrice_YCbCr[i][j]; // on considere comme étant un bloc juste dans un premier temps

            if (bloc_actu.Y != NULL){
                struct huff_table *tab_AC_Y = huffman_table_build(htables_nb_symb_per_lengths[AC][Y], htables_symbols[AC][Y], htables_nb_symbols[AC][Y]);
                struct huff_table *tab_DC_Y = huffman_table_build(htables_nb_symb_per_lengths[DC][Y], htables_symbols[DC][Y], htables_nb_symbols[DC][Y]);
            
                int16_t **Y_DCT = DCT(bloc_actu.Y, 8);
            }

             if (bloc_actu.Cb != NULL){
                struct huff_table *tab_AC_Cb = huffman_table_build(htables_nb_symb_per_lengths[AC][Cb], htables_symbols[AC][Cb], htables_nb_symbols[AC][Cb]);
                struct huff_table *tab_DC_Cb = huffman_table_build(htables_nb_symb_per_lengths[DC][Cb], htables_symbols[DC][Cb], htables_nb_symbols[DC][Cb]);
            
                int16_t **Cb_DCT = DCT(bloc_actu.Y, 8);
            }
            if (bloc_actu.Cr != NULL){
                struct huff_table *tab_AC_Y = huffman_table_build(htables_nb_symb_per_lengths[AC][Cr], htables_symbols[AC][Cr], htables_nb_symbols[AC][Cr]);
                struct huff_table *tab_DC_Y = huffman_table_build(htables_nb_symb_per_lengths[DC][Cr], htables_symbols[DC][Cr], htables_nb_symbols[DC][Cr]);
            
                int16_t **CrDCT = DCT(bloc_actu.Y, 8);
            }

        }   
           
    }
}



void free_DC_vect(struct DC_MCU *vect_DC)
{
    if (vect_DC->Y != NULL) free(vect_DC->Y);
    if (vect_DC->Cb != NULL) free(vect_DC->Cb);
    if (vect_DC->Cr != NULL) free(vect_DC->Cr);
    free(vect_DC);
}

/*void main(void)
{
    int16_t vector[34] = {0xffff, 0x0001, 0x0000, 0x0000, 0x0000, 0x0001, 0x0000, 0x0000, 0x0001,0x0000, 0x0000, 0x0000,0x0000, 0x0000, 0x0000,0x0000, 0x0000, 0x0000,0x0000, 0x0000, 0x0000,0x0000, 0x0000, 0x0000,0x0000, 0x0000, 0x0000,0x0000, 0x0000, 0x0000,0x0000, 0x0000, 0x0000, 0xffff};
    struct RLE_to_bitstream *out = code_AC_RLE(vector);
    for (uint8_t i = 0; i<5; i++) printf("%x%x %x ", out[i].first_byte, out[i].second_byte, out[i].index);
    printf("\n");

    struct pixel_R **matric_pixel = malloc(16*sizeof(struct pixel_R*));
    for (uint8_t i=0; i<16; i++){
        matric_pixel[i] = malloc(16*sizeof(struct pixel_R));
        for (uint8_t j=0; j<16; j++){
            matric_pixel[i][j].R = rand() % 256;
            matric_pixel[i][j].G = rand() % 256;
            matric_pixel[i][j].B = rand() % 256;
        }
    }
    struct MCU_s *notre_matrice = creer_MCU(matric_pixel, 2,2);
    // génération de la nouvelle matrice avec les composantes YCbCr
    struct MCU_YCbCr *matrice_YCbCr = MCU_to_YCbCr(notre_matrice);
    struct MCU_vector *vect = MCU_to_vector(matrice_YCbCr);
    for (uint8_t i = 0; i<64; i++) printf("%04hx   ", vect->vector_Cr.vector[0][i]);
    printf("\n");
}*/