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
    /*
        On cree la structure avec laquelle on travaille qui est matrix_MCU
    */
    struct matrix *matrice_brut_matrix = malloc(sizeof(struct matrix));

    matrice_brut_matrix->col = *nb_colonne;
    matrice_brut_matrix->lines = *nb_ligne;
    matrice_brut_matrix->matrice = matrice_brute;

    // création de la matrix pour le module decoupe

    struct matrix *JSP = malloc(sizeof(struct matrix));
    JSP->col = *nb_colonne;
    JSP->lines = *nb_ligne;
    JSP->matrice = matrice_brute;

    struct matrix_MCU notre_matrice_MCU = image_to_MCU(*JSP, 1, 1); // testé et validé 

    struct MCU_YCbCr *mcu_YCbCr = MCU_to_YCbCr(&(notre_matrice_MCU.matrice[0][0]));
    //affiche_matrice(mcu_YCbCr->blocs[0].Y, 8, 8);
    //printf("############# \n");
    int16_t **notre_MCU_DCT = DCT(mcu_YCbCr->blocs[0].Y, 8);
    //printf("La DCT est : \n ");
    //affiche_matrice(notre_MCU_DCT, 8, 8);

    mcu_YCbCr->blocs[0].Y = notre_MCU_DCT;

    struct MCU_vector *vect = MCU_to_vector(mcu_YCbCr);
    // printf("ok\n");

    // for (uint8_t i = 0; i<8; i++){
    //     for (uint8_t j = 0; j < 8; j++)
    //     {
    //         printf("%04hx \t", vect->vector_Y.vector[0][i*8 + j]);
    //     }
    //     printf("\n");
        
    // }    
    // printf("ok\n");
    
    
    struct DC_MCU *vector_DC_MCU = code_DC(vect);
    
    struct bitstream *notre_bitstream = jpeg_get_bitstream(notre_jpeg);

//    bitstream_write_bits(notre_bitstream, 0xffc0, 16, 1);

    struct magnitude_s notre_magnitude = get_magnitude(vector_DC_MCU->Y[0]);
    uint8_t *long_chemin = malloc(sizeof(uint8_t));
    uint32_t val_bitstream = huffman_table_get_path(table_DC_Y, notre_magnitude.magnitude , long_chemin);

    
     bitstream_write_bits(notre_bitstream, val_bitstream, *long_chemin, 0);
     bitstream_write_bits(notre_bitstream, notre_magnitude.indice, notre_magnitude.magnitude, 0);

    // int16_t vect[64]= {0x18, 0x00, 0xffff, 0xffbe, 0x00, 0xffe9, 0x00, 0xfff0, 0x00, 0xffce, 0x001f, 0x00, 0xfff2, 0x00, 0x00, 0x00, 0x001a, 0x00, 0xffed, 0x00, 0xfff9, 0xfff8, 0x00, 0x10, 0x00, 0x0d, 0x00, 0xfffe, 0x00, 0xfffb, 0x00, 0x01, 0x00, 0x02, 0x00, 0x0c, 0x00, 0x00, 0x00, 0x0b, 0x00, 0x0d, 0x00, 0x00, 0x00, 0x00, 0x03, 0x00, 0xfffb, 0x00, 0xfffe, 0x00, 0x08, 0x00, 0x00, 0xfffa, 0x00};
    // for (uint8_t i = 0; i<28; i++){
         //uint32_t value1 = huffman_table_get_path(table_AC_Y, concatenate_hex((vector_AC_MCU[i]).first_byte, (vector_AC_MCU[i]).second_byte), long_chemin);
         code_AC_RLE(vect->vector_Y.vector[0], notre_bitstream, table_AC_Y);

    //     //bitstream_flush(notre_bitstream);
    //     //printf("%04hx %04hx \n", value1, vector_AC_MCU[i].index);
     //}
    bitstream_flush(notre_bitstream);
    jpeg_write_footer(notre_jpeg);
    
    jpeg_destroy(notre_jpeg);
    return EXIT_SUCCESS;
}