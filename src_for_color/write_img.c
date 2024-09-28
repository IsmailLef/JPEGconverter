#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>

#include "traitement.h"
#include "PPM_FILE.h"
#include "decoupe_pgm.h"
#include "PGM_FILE.h"
#include "DCT.h"
#include "zigzag.h"
#include "quantification.h"
#include "magnitude.h"
#include "jpeg_writer.h"
#include "htables.h"
#include "huffman.h"
#include "RGB.h"
#include "RLE.h"
#include "bitstream.h"


int main(int argc, char * argv[])

{   struct jpeg *notre_jpeg = jpeg_create();
    
    char ** nv_nom = malloc(sizeof(char *));
    char ** chemin_image = malloc(sizeof(char*));
    uint8_t ** facteurs_echantillonages = malloc(sizeof(uint8_t *));

    traitement(argc, argv, nv_nom, chemin_image, facteurs_echantillonages); // on retrouve par cette ligne les données de la ligne de commande

    jpeg_set_ppm_filename(notre_jpeg, *chemin_image);
    jpeg_set_jpeg_filename( notre_jpeg , *nv_nom);
    
    uint32_t * nb_colonne = malloc(sizeof(uint32_t));
    uint32_t * nb_ligne = malloc(sizeof(uint32_t));

    /*
        on retrouve la matrice brute des données de l'image donné comme parametre,
        ainsi que le nbr de colonne et de ligne de l'image
    */

    int16_t **matrice_brute = PGM_FILE(*chemin_image, nb_colonne, nb_ligne);


    jpeg_set_image_height(notre_jpeg, *nb_ligne);
    jpeg_set_image_width(notre_jpeg, *nb_colonne);

    jpeg_set_nb_components(notre_jpeg, 1);

    jpeg_set_sampling_factor(notre_jpeg, Y, H, 1);
    jpeg_set_sampling_factor(notre_jpeg, Y, V, 1);

    struct huff_table *table_AC_Y = huffman_table_build(htables_nb_symb_per_lengths[1][0], htables_symbols[1][0], htables_nb_symbols[1][0]); // normalement on devait ajouter [16]
    struct huff_table *table_DC_Y = huffman_table_build(htables_nb_symb_per_lengths[0][0], htables_symbols[0][0], htables_nb_symbols[0][0]); // normalement on devait ajouter [16]

    jpeg_set_huffman_table(notre_jpeg, AC, Y, table_AC_Y);
    jpeg_set_huffman_table(notre_jpeg, DC, Y, table_DC_Y);



    jpeg_set_quantization_table(notre_jpeg, Y, quantification_table_Y);

    jpeg_write_header(notre_jpeg);
    
    struct bitstream *notre_bitstream = jpeg_get_bitstream(notre_jpeg);

    // uint8_t *hftable = huffman_table_get_symbols(table_AC_Y);
    // for (uint16_t i = 0; i<60; i++) printf("%u\n",hftable[i]);
     /*
         On cree la structure avec laquelle on travaille qui est matrix_MCU
     */
    struct matrix *matrice_brut_matrix = malloc(sizeof(struct matrix));

    matrice_brut_matrix->col = *nb_colonne;
    matrice_brut_matrix->lines = *nb_ligne;
    matrice_brut_matrix->matrice = matrice_brute;

    struct matrix_MCU notre_matrice_MCU = image_to_MCU(*matrice_brut_matrix, 1, 1);

    free_matrix(matrice_brute, matrice_brut_matrix->lines);
    free(matrice_brut_matrix);

    struct matrix_MCU_YCbCr *notre_matrice_MCU_YCbCr = matrix_MCU_to_YCbCr(notre_matrice_MCU);
    int16_t old_value = 0;
    int16_t new_value = 0;
    


    for (uint32_t i = 0; i<notre_matrice_MCU_YCbCr->bloc_line; i++){
        for (uint32_t j = 0; j<notre_matrice_MCU_YCbCr->bloc_col; j++){
            int16_t **notre_MCU_DCT = DCT(notre_matrice_MCU_YCbCr->matrice[i][j].blocs[0].Y, 8);
            notre_matrice_MCU_YCbCr->matrice[i][j].blocs[0].Y = notre_MCU_DCT;
            struct MCU_vector *vect = MCU_to_vector(&(notre_matrice_MCU_YCbCr->matrice[i][j]));

            //#################################################################################################
            // if (i == notre_matrice_MCU_YCbCr->bloc_line - 1 && j == notre_matrice_MCU_YCbCr->bloc_col - 1){
            // for (uint8_t k = 0; k<8; k++){
            //         for (uint8_t l = 0; l < 8; l++){
            //             printf("%04hx \t", vect->vector_Y.vector[0][k*8+l]);
            //         }
            //     printf("\n");
            //     }    
            // }
            //#################################################################################################

            
            new_value = vect->vector_Y.vector[0][0] - old_value;
            //printf("newvalue %x\n", new_value);
            struct magnitude_s notre_magnitude = get_magnitude(new_value);
            //struct DC_MCU *vector_DC_MCU = code_DC(vect);
            //struct magnitude_s notre_magnitude = get_magnitude(vector_DC_MCU->Y[0]);
            uint8_t *long_chemin = malloc(sizeof(uint8_t));
            uint32_t val_bitstream = huffman_table_get_path(table_DC_Y, notre_magnitude.magnitude , long_chemin);
            //printf("############################################################################################\n");
            //printf("DC: \n");
            //printf("value = %i, magnitude = %i, index = %i \n", old_value, notre_magnitude.magnitude, notre_magnitude.indice );
            //printf("bitstream => writing %i over %i bits \n", val_bitstream, *long_chemin);
            bitstream_write_bits(notre_bitstream, val_bitstream, *long_chemin, 0);
            bitstream_write_bits(notre_bitstream, notre_magnitude.indice, notre_magnitude.magnitude, 0);
            code_AC_RLE(vect->vector_Y.vector[0], notre_bitstream, table_AC_Y);
            old_value = vect->vector_Y.vector[0][0];
            
            //#################CONTEXTE FREE TOUTES LES STRUCTURES CRÉÉES#####################################
            free_MCU(notre_matrice_MCU.matrice[i][j]);
            free_MCU_YCbCr(notre_matrice_MCU_YCbCr->matrice[i][j]);
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
            free(long_chemin);
            
        }
    }

    free(notre_matrice_MCU_YCbCr);

    bitstream_flush(notre_bitstream);
    jpeg_write_footer(notre_jpeg);
    
    jpeg_destroy(notre_jpeg);

    return EXIT_SUCCESS;
}