#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "traitement.h"
#include "PPM_FILE.h"
#include "DCT.h"
#include "zigzag.h"
#include "quantification.h"
#include "magnitude.h"
#include "jpeg_writer.h"
#include "htables.h"
#include "huffman.h"
#include "downsampling.h"
#include "RGB_Couleur.h"
#include "RLE.h"
#include "bitstream.h"
#include "downsampling.h"


#define PI 3.14159265358979323846264338

void free_elements_main(struct fact_echant *fact1, struct fact_echant *fact2,
                        uint32_t *nb_colonne, uint32_t *nb_ligne, char **nv_nom,
                        char **chemin_image, uint8_t **facteurs_echantillonages,
                        struct matrix_MCU_C notre_matrice_MCU, struct matrix_MCU_YCbCr *notre_matrice_MCU_YCbCr,
                        uint8_t *long_chemin_Y, uint8_t *long_chemin_Cb, uint8_t *long_chemin_Cr){
    free(long_chemin_Y);
    free(long_chemin_Cb);
    free(long_chemin_Cr);
    free(fact1);
    free(fact2);
    free(nb_colonne);
    free(nb_ligne);
    free(nv_nom);
    free(chemin_image);
    free(*facteurs_echantillonages);
    free(facteurs_echantillonages);
    free(notre_matrice_MCU.matrice);
    for (uint32_t i = 0; i<notre_matrice_MCU_YCbCr->bloc_line; i++) free(notre_matrice_MCU_YCbCr->matrice[i]);
    free(notre_matrice_MCU_YCbCr->matrice);
    free(notre_matrice_MCU_YCbCr);
}


void free_in_loop(struct matrix_MCU_YCbCr *notre_matrice_MCU_YCbCr, struct MCU_vector *vect,
                  struct DC_MCU *vector_DC_MCU, uint32_t i, uint32_t j, struct fact_echant *fact1,
                  struct fact_echant *fact2){
    free_MCU_YCbCr(notre_matrice_MCU_YCbCr->matrice[i][j], fact1, fact2);
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
        free_DC_vect(vector_DC_MCU);
}


/*
    Ceci c'est pour les photos en couleuurs :=) 
*/

int main(int argc, char * argv[])

{   struct jpeg *notre_jpeg = jpeg_create();
    
    char ** nv_nom = malloc(sizeof(char *));
    char ** chemin_image = malloc(sizeof(char*));
    uint8_t ** facteurs_echantillonages = malloc(sizeof(uint8_t *));
    *facteurs_echantillonages = calloc(6, sizeof(uint8_t));
    traitement(argc, argv, nv_nom, chemin_image, facteurs_echantillonages); // on retrouve par cette ligne les données de la ligne de commande

    struct fact_echant *fact1 = malloc(sizeof(struct fact_echant));
    struct fact_echant *fact2 = malloc(sizeof(struct fact_echant));

    fact1->h = (*facteurs_echantillonages)[0];
    fact1->v = (*facteurs_echantillonages)[1];
    fact2->h = (*facteurs_echantillonages)[2];
    fact2->v = (*facteurs_echantillonages)[3];
    //fact2 et fact3 sont égaux.

    jpeg_set_ppm_filename(notre_jpeg, *chemin_image);
    jpeg_set_jpeg_filename( notre_jpeg , *nv_nom);
    
    uint32_t *nb_colonne = malloc(sizeof(uint32_t));
    uint32_t *nb_ligne = malloc(sizeof(uint32_t));
    /*
        on retrouve la matrice brute des données de l'image donné comme parametre,
        ainsi que le nbr de colonne et de ligne de l'image
    */

    struct pixel_R ***matrice_brute = donnes_brut(*chemin_image, nb_colonne, nb_ligne); // pour les couleurs struct pixel R



    jpeg_set_image_height(notre_jpeg, *nb_ligne);
    jpeg_set_image_width(notre_jpeg, *nb_colonne);

    jpeg_set_nb_components(notre_jpeg, 3);

    jpeg_set_sampling_factor(notre_jpeg, Y, H, fact1->h);
    jpeg_set_sampling_factor(notre_jpeg, Y, V, fact1->v);

    jpeg_set_sampling_factor(notre_jpeg, Cb, H, fact2->h);
    jpeg_set_sampling_factor(notre_jpeg, Cb, V, fact2->v);

    jpeg_set_sampling_factor(notre_jpeg, Cr, H, fact2->h);
    jpeg_set_sampling_factor(notre_jpeg, Cr, V, fact2->v);

    struct huff_table *table_AC_Y = huffman_table_build(htables_nb_symb_per_lengths[AC][Y], htables_symbols[AC][Y], htables_nb_symbols[AC][Y]); 
    struct huff_table *table_DC_Y = huffman_table_build(htables_nb_symb_per_lengths[DC][Y], htables_symbols[DC][Y], htables_nb_symbols[DC][Y]); 

    struct huff_table *table_AC_Cb = huffman_table_build(htables_nb_symb_per_lengths[AC][Cb], htables_symbols[AC][Cb], htables_nb_symbols[AC][Cb]); 
    struct huff_table *table_DC_Cb = huffman_table_build(htables_nb_symb_per_lengths[DC][Cb], htables_symbols[DC][Cb], htables_nb_symbols[DC][Cb]); 

    struct huff_table *table_AC_Cr = huffman_table_build(htables_nb_symb_per_lengths[AC][Cr], htables_symbols[AC][Cr], htables_nb_symbols[AC][Cr]); 
    struct huff_table *table_DC_Cr = huffman_table_build(htables_nb_symb_per_lengths[DC][Cr], htables_symbols[DC][Cr], htables_nb_symbols[DC][Cr]);

    jpeg_set_huffman_table(notre_jpeg, AC, Y, table_AC_Y);
    jpeg_set_huffman_table(notre_jpeg, DC, Y, table_DC_Y);

    jpeg_set_huffman_table(notre_jpeg, AC, Cb, table_AC_Cb);
    jpeg_set_huffman_table(notre_jpeg, DC, Cb, table_DC_Cb);

    jpeg_set_huffman_table(notre_jpeg, AC, Cr, table_AC_Cr);
    jpeg_set_huffman_table(notre_jpeg, DC, Cr, table_DC_Cr);



    jpeg_set_quantization_table(notre_jpeg, Y, quantification_table_Y);

    jpeg_set_quantization_table(notre_jpeg, Cb, quantification_table_CbCr);

    jpeg_set_quantization_table(notre_jpeg, Cr, quantification_table_CbCr);

    jpeg_write_header(notre_jpeg);

    
    struct bitstream *notre_bitstream = jpeg_get_bitstream(notre_jpeg);


    struct matrix_C *matrice_brut_matrix = malloc(sizeof( struct matrix_C ));

    matrice_brut_matrix->col = *nb_colonne;
    matrice_brut_matrix->lines = *nb_ligne;
    matrice_brut_matrix->matrice = matrice_brute;


    uint32_t bloc_MCU_col_Y = (*facteurs_echantillonages)[0]; // = facteurs_echantillonages[0]
    uint32_t bloc_MCU_lines_Y = (*facteurs_echantillonages)[1];//  = facteurs_echantillonages[1]

    uint32_t bloc_MCU_col_C = (*facteurs_echantillonages)[2]; // = facteurs_echantillonages[2]
    uint32_t bloc_MCU_lines_C = (*facteurs_echantillonages)[3];//  = facteurs_echantillonages[3]

    struct matrix_MCU_C notre_matrice_MCU = image_to_MCU_Couleur(*matrice_brut_matrix, bloc_MCU_lines_Y, bloc_MCU_col_Y); // c'est notre matrice des MCUs contenant les composantes RGB


    struct matrix_MCU_YCbCr *notre_matrice_MCU_YCbCr = matrix_MCU_to_YCbCr_Couleur(notre_matrice_MCU, fact1, fact2, fact2); // la matrice des MCUs contenant les composantes YCbC


    free_matrix_Couleur(matrice_brute, *nb_ligne, *nb_colonne);
    free(matrice_brut_matrix);

    int16_t old_value_Y = 0;
    int16_t new_value_Y = 0;

    int16_t old_value_Cb = 0;
    int16_t new_value_Cb = 0;

    int16_t old_value_Cr = 0;
    int16_t new_value_Cr = 0;

    uint8_t *long_chemin_Y = malloc(sizeof(uint8_t));
    uint8_t *long_chemin_Cb = malloc(sizeof(uint8_t));
    uint8_t *long_chemin_Cr = malloc(sizeof(uint8_t));
            
    
    //Création du tableau des cos pour accélérer l'accès aux valeurs
    float **tab_cos = calloc(8, sizeof(float *)); 
    for(uint8_t x=0 ; x < 8 ; x++){
        tab_cos[x] = calloc(8, sizeof(float));
        for(uint8_t y=0 ; y < 8 ; y++) tab_cos[x][y]=(cos((float)((2*y +1)*x*PI)/(2*8))); 
    }




    for (uint32_t i = 0; i<notre_matrice_MCU_YCbCr->bloc_line; i++){
        for (uint32_t j = 0; j<notre_matrice_MCU_YCbCr->bloc_col; j++){

            for (uint32_t k = 0; k<bloc_MCU_col_Y*bloc_MCU_lines_Y; k++){
                int16_t **notre_MCU_DCT_Y = DCT(notre_matrice_MCU_YCbCr->matrice[i][j].blocs[k].Y, 8, tab_cos);
                free_matrix(notre_matrice_MCU_YCbCr->matrice[i][j].blocs[k].Y, 8);
                notre_matrice_MCU_YCbCr->matrice[i][j].blocs[k].Y = notre_MCU_DCT_Y;
            }
        if ((fact1->h == fact2->h) && (fact1->v*0.5 == fact2->v)){ //Sous échantillonage horizontal 2x2 a le deuxième bloc Cr NULL donc on ne les parcours pas 
                int16_t **notre_MCU_DCT_Cb = DCT(notre_matrice_MCU_YCbCr->matrice[i][j].blocs[0].Cb, 8, tab_cos); 
                free_matrix(notre_matrice_MCU_YCbCr->matrice[i][j].blocs[0].Cb, 8);
                notre_matrice_MCU_YCbCr->matrice[i][j].blocs[0].Cb = notre_MCU_DCT_Cb;
            }
            else{
            for (uint32_t k = 0; k<bloc_MCU_col_C*bloc_MCU_col_C; k++){
                int16_t **notre_MCU_DCT_Cb = DCT(notre_matrice_MCU_YCbCr->matrice[i][j].blocs[k].Cb, 8, tab_cos);
                free_matrix(notre_matrice_MCU_YCbCr->matrice[i][j].blocs[k].Cb, 8);
                notre_matrice_MCU_YCbCr->matrice[i][j].blocs[k].Cb = notre_MCU_DCT_Cb;
            }
        }
            if ((fact1->h == fact2->h) && (fact1->v*0.5 == fact2->v)){ //Sous échantillonage horizontal 2x2 a le deuxième bloc Cr NULL donc on ne les parcours pas 
                int16_t **notre_MCU_DCT_Cr = DCT(notre_matrice_MCU_YCbCr->matrice[i][j].blocs[0].Cr, 8, tab_cos); 
                free_matrix(notre_matrice_MCU_YCbCr->matrice[i][j].blocs[0].Cr, 8);
                notre_matrice_MCU_YCbCr->matrice[i][j].blocs[0].Cr = notre_MCU_DCT_Cr;
            }
            else{
                for (uint32_t k = 0; k<bloc_MCU_col_C*bloc_MCU_col_C; k++){
                    int16_t **notre_MCU_DCT_Cr = DCT(notre_matrice_MCU_YCbCr->matrice[i][j].blocs[k].Cr, 8, tab_cos); 
                    free_matrix(notre_matrice_MCU_YCbCr->matrice[i][j].blocs[k].Cr, 8);
                    notre_matrice_MCU_YCbCr->matrice[i][j].blocs[k].Cr = notre_MCU_DCT_Cr;
                }
            }
            struct MCU_vector *vect = MCU_to_vector(&(notre_matrice_MCU_YCbCr->matrice[i][j]), fact1, fact2);

            struct DC_MCU *vector_DC_MCU = code_DC(vect);

            
            
            new_value_Y = vector_DC_MCU->Y[0] - old_value_Y;
            vector_DC_MCU->Y[0] = new_value_Y;

            new_value_Cb = vector_DC_MCU->Cb[0] - old_value_Cb;
            vector_DC_MCU->Cb[0] = new_value_Cb;


            new_value_Cr = vector_DC_MCU->Cr[0] - old_value_Cr;
            vector_DC_MCU->Cr[0] = new_value_Cr;


        

            if (bloc_MCU_col_Y*bloc_MCU_lines_Y == 2){
                
                    struct magnitude_s magnitude_Y = get_magnitude(vector_DC_MCU->Y[0]);
                    uint32_t val_bitstream_Y = huffman_table_get_path(table_DC_Y, magnitude_Y.magnitude , long_chemin_Y);
                    bitstream_write_bits(notre_bitstream, val_bitstream_Y, *long_chemin_Y, 0);
                    bitstream_write_bits(notre_bitstream, magnitude_Y.indice, magnitude_Y.magnitude, 0);
                    code_AC_RLE(vect->vector_Y.vector[0], notre_bitstream, table_AC_Y);


                    magnitude_Y = get_magnitude(vector_DC_MCU->Y[1]);
                    val_bitstream_Y = huffman_table_get_path(table_DC_Y, magnitude_Y.magnitude , long_chemin_Y);
                    bitstream_write_bits(notre_bitstream, val_bitstream_Y, *long_chemin_Y, 0);
                    bitstream_write_bits(notre_bitstream, magnitude_Y.indice, magnitude_Y.magnitude, 0);
                    code_AC_RLE(vect->vector_Y.vector[1], notre_bitstream, table_AC_Y);


                    struct magnitude_s magnitude_Cb = get_magnitude(vector_DC_MCU->Cb[0]);
                    uint32_t val_bitstream_Cb = huffman_table_get_path(table_DC_Cb, magnitude_Cb.magnitude , long_chemin_Cb);
                    bitstream_write_bits(notre_bitstream, val_bitstream_Cb, *long_chemin_Cb, 0);
                    bitstream_write_bits(notre_bitstream, magnitude_Cb.indice, magnitude_Cb.magnitude, 0);
                    code_AC_RLE(vect->vector_Cb.vector[0], notre_bitstream, table_AC_Cb);


                    struct magnitude_s magnitude_Cr = get_magnitude(vector_DC_MCU->Cr[0]);
                    uint32_t val_bitstream_Cr = huffman_table_get_path(table_DC_Cr, magnitude_Cr.magnitude , long_chemin_Cr);
                    bitstream_write_bits(notre_bitstream, val_bitstream_Cr, *long_chemin_Cr, 0);
                    bitstream_write_bits(notre_bitstream, magnitude_Cr.indice, magnitude_Cr.magnitude, 0);
                    code_AC_RLE(vect->vector_Cr.vector[0], notre_bitstream, table_AC_Cr);
            }
            else{
                for (uint32_t k = 0; k<bloc_MCU_col_Y*bloc_MCU_lines_Y; k++){
                    struct magnitude_s magnitude_Y = get_magnitude(vector_DC_MCU->Y[k]);
                    uint32_t val_bitstream_Y = huffman_table_get_path(table_DC_Y, magnitude_Y.magnitude , long_chemin_Y);
                    bitstream_write_bits(notre_bitstream, val_bitstream_Y, *long_chemin_Y, 0);
                    bitstream_write_bits(notre_bitstream, magnitude_Y.indice, magnitude_Y.magnitude, 0);
                    code_AC_RLE(vect->vector_Y.vector[k], notre_bitstream, table_AC_Y);

                }

                for (uint32_t k = 0; k<bloc_MCU_lines_C*bloc_MCU_col_C; k++){
                    struct magnitude_s magnitude_Cb = get_magnitude(vector_DC_MCU->Cb[k]);
                    uint32_t val_bitstream_Cb = huffman_table_get_path(table_DC_Cb, magnitude_Cb.magnitude , long_chemin_Cb);
                    bitstream_write_bits(notre_bitstream, val_bitstream_Cb, *long_chemin_Cb, 0);
                    bitstream_write_bits(notre_bitstream, magnitude_Cb.indice, magnitude_Cb.magnitude, 0);
                    code_AC_RLE(vect->vector_Cb.vector[k], notre_bitstream, table_AC_Cb);
                }


                for (uint32_t k = 0; k<bloc_MCU_lines_C*bloc_MCU_col_C; k++){
                    struct magnitude_s magnitude_Cr = get_magnitude(vector_DC_MCU->Cr[k]);
                    uint32_t val_bitstream_Cr = huffman_table_get_path(table_DC_Cr, magnitude_Cr.magnitude , long_chemin_Cr);
                    bitstream_write_bits(notre_bitstream, val_bitstream_Cr, *long_chemin_Cr, 0);
                    bitstream_write_bits(notre_bitstream, magnitude_Cr.indice, magnitude_Cr.magnitude, 0);
                    code_AC_RLE(vect->vector_Cr.vector[k], notre_bitstream, table_AC_Cr);
                }
            }
            old_value_Y = vect->vector_Y.vector[vect->vector_Y.byte_lenght - 1][0];
            old_value_Cb = vect->vector_Cb.vector[vect->vector_Cb.byte_lenght - 1][0];
            old_value_Cr = vect->vector_Cr.vector[vect->vector_Cr.byte_lenght - 1][0];


            //freeing structures
            free_in_loop(notre_matrice_MCU_YCbCr, vect, vector_DC_MCU, i, j, fact1, fact2);
        }
    }
    

    free_matrix(tab_cos, 8);
    free_elements_main(fact1, fact2, nb_colonne, nb_ligne, nv_nom, chemin_image, facteurs_echantillonages, notre_matrice_MCU, notre_matrice_MCU_YCbCr, long_chemin_Y, long_chemin_Cb, long_chemin_Cr);
    

    bitstream_flush(notre_bitstream);
    jpeg_write_footer(notre_jpeg);

    huffman_table_destroy(table_AC_Y);
    huffman_table_destroy(table_DC_Y);
    huffman_table_destroy(table_AC_Cb);
    huffman_table_destroy(table_DC_Cb);
    huffman_table_destroy(table_AC_Cr);
    huffman_table_destroy(table_DC_Cr);
    
    jpeg_destroy(notre_jpeg);

    return EXIT_SUCCESS;
}