#include <stdlib.h> 
#include <stdint.h> 
#include <stdio.h> 
#include <string.h> 
#include <ctype.h> 
#include <math.h> 
#include <stdbool.h>

#include "huffman_t.h"
#include "htables.h"
#include "bitstream_t.h"
#include "jpeg_writer_t.h"



// struct jpeg{
//     char *name_fppm;    //     le nom de fichier PPM ppm_filename
//     uint32_t hauteur ;          //     la hauteur de l image traitée
//     uint32_t largeur ;      //     la largeur de l image traitée
//     uint8_t nbr_components;          //     le nombre de composantes de couleur de l image
//     char *name_fjpeg   ;      //     le nom du fichier de sortie jpeg_filename
//     struct couleur_composante *Y   ;     //     le facteur d échantillonnage sampling_factor
//     struct couleur_composante *Cr;
//     struct couleur_composante *Cb;    
//     uint8_t *quant_table_Y ;
//     uint8_t *quant_table_CbCr ;
//     struct bitstream * stream;
// };





struct couleur_composante{
   uint8_t h ;
   uint8_t v ; 
   struct huff_table *htable_ac ;
   struct huff_table *htable_dc ;

};


// /* Type énuméré représentant les composantes de couleur YCbCr. */
// enum color_component
// {
//     Y,
//     Cb,
//     Cr,
//     NB_COLOR_COMPONENTS
// };

// /*
//     Type énuméré représentant les types de composantes
//     fréquentielles (DC ou AC).
// */
// enum sample_type
// {
//     DC,
//     AC,
//     NB_SAMPLE_TYPES
// };

// /*
//     Type énuméré représentant la direction des facteurs
//     d'échantillonnage (H pour horizontal, V pour vertical).
// */
// enum direction
// {
//     H,
//     V,
//     NB_DIRECTIONS
// };



/* Alloue et retourne une nouvelle structure jpeg. */
struct jpeg *jpeg_create(void){
     struct jpeg *new_jpeg = malloc(sizeof(struct jpeg));
     new_jpeg->Y = malloc(sizeof(struct couleur_composante));
     new_jpeg->Cr = malloc(sizeof(struct couleur_composante));
     new_jpeg->Cb = malloc(sizeof(struct couleur_composante));

     new_jpeg->Y->htable_ac = malloc(sizeof(struct huff_table));
     new_jpeg->Y->htable_dc = malloc(sizeof(struct huff_table));
     new_jpeg->Cb->htable_ac = malloc(sizeof(struct huff_table));
     new_jpeg->Cb->htable_dc = malloc(sizeof(struct huff_table));
     new_jpeg->Cr->htable_ac = malloc(sizeof(struct huff_table));
     new_jpeg->Cr->htable_dc = malloc(sizeof(struct huff_table));

     new_jpeg->quant_table_CbCr = calloc(64,sizeof(uint8_t));
     new_jpeg->quant_table_Y = calloc(64,sizeof(uint8_t));
     //new_jpeg->stream = malloc(sizeof(struct bitstream ));



     return(new_jpeg);


}

/*
    Détruit une structure jpeg.
    Toute la mémoire qui lui est associée est libérée.
*/
void jpeg_destroy(struct jpeg *jpg){
     
    free(jpg->Y->htable_ac);
    free(jpg->Y->htable_dc);
    free(jpg->Cb->htable_ac);
    free(jpg->Cb->htable_dc);
    free(jpg->Cr->htable_ac);
    free(jpg->Cr->htable_dc);
    free(jpg->Y);
    free(jpg->Cr);
    free(jpg->Cr);
    free(jpg->quant_table_CbCr);
    free(jpg->quant_table_Y);
    //free(jpg->stream);
    free(jpg); 
}

/*
    Ecrit tout l'en-tête JPEG dans le fichier de sortie à partir des
    informations contenues dans la structure jpeg passée en paramètre.
    En sortie, le bitstream est positionné juste après l'écriture de
    l'en-tête SOS, à l'emplacement du premier octet de données brutes à écrire.
*/
void jpeg_write_header(struct jpeg *jpg){

//------------------ SOI
    jpg->stream=creat_bitstream( (const char *) (jpg->name_fjpeg));

    printf("bitstream\n");
    bitstream_write_bits(jpg->stream , 0xd8 , 8 , true );
    printf("on a écrit \n");
//------------------ APP0
    bitstream_write_bits(jpg->stream , 0xe0 , 16 , true );
    bitstream_write_bits(jpg->stream , 0x0010 , 16 , false );
    bitstream_write_bits(jpg->stream ,  0x4a46494600, 40 , false );
    bitstream_write_bits(jpg->stream ,  0x01, 8 , false );
    bitstream_write_bits(jpg->stream ,  0x01, 8 , false );
    bitstream_write_bits(jpg->stream ,  0x00, 56 , false );

//------------------------COM 
    bitstream_write_bits(jpg->stream , 0xfe , 16 , true );
    bitstream_write_bits(jpg->stream , 2 , 16 , false );
    

//----------------------------DQT

//pour quant_Y
    bitstream_write_bits(jpg->stream , 0xdb , 16 , true );
    bitstream_write_bits(jpg->stream , 0x0043 , 16 , false );
    bitstream_write_bits(jpg->stream , 0 , 4 , false );
    bitstream_write_bits(jpg->stream , 0 , 4 , false );
    for(uint32_t i = 0 ; i <64 ; i++){
        bitstream_write_bits(jpg->stream , jpg->quant_table_Y[i] , 8 , false );
    }

if(jpg->nbr_components==3){
//pour quant_Cb_Cr
    bitstream_write_bits(jpg->stream , 0xdb , 16 , true );
    bitstream_write_bits(jpg->stream , 0x0043 , 16 , false );
    bitstream_write_bits(jpg->stream , 0 , 4 , false );
    bitstream_write_bits(jpg->stream , 1 , 4 , false );
    for(uint32_t i = 0 ; i <64 ; i++){
        bitstream_write_bits(jpg->stream , jpg->quant_table_CbCr[i] , 8 , false );
    }
    
}


//------------------------------------SOF 

    bitstream_write_bits(jpg->stream , 0xc0 , 16 , true );
    bitstream_write_bits(jpg->stream , 8+(3*jpg->nbr_components) , 16 , false );
    bitstream_write_bits(jpg->stream , 0x08 , 8 , false );
    bitstream_write_bits(jpg->stream , jpg->hauteur , 16 , false );
    bitstream_write_bits(jpg->stream , jpg->largeur , 16 , false );
    bitstream_write_bits(jpg->stream , jpg->nbr_components , 8 , false );

// Pour Y      
        bitstream_write_bits(jpg->stream , 1 , 8 , false ); // identifiant ic
        bitstream_write_bits(jpg->stream , jpg->Y->h , 4 , false ); // Facteur d'échantillonnage (sampling factor) horizontal, de 1 à 4
        bitstream_write_bits(jpg->stream , jpg->Y->v , 4 , false ); // Facteur d'échantillonnage (sampling factor) vertical, de 1 à 4
        bitstream_write_bits(jpg->stream , 0 , 8 , false ); // Table de quantification iQ associée, de 0 à 3

if(jpg->nbr_components==3){
    // pour Cb
        bitstream_write_bits(jpg->stream , 2 , 8 , false ); // identifiant ic
        bitstream_write_bits(jpg->stream , jpg->Cb->h , 4 , false ); // Facteur d'échantillonnage (sampling factor) horizontal, de 1 à 4
        bitstream_write_bits(jpg->stream , jpg->Cb->v , 4 , false ); // Facteur d'échantillonnage (sampling factor) vertical, de 1 à 4
        bitstream_write_bits(jpg->stream , 1 , 8 , false ); // Table de quantification iQ associée, de 0 à 3

    // pour Cr
        bitstream_write_bits(jpg->stream , 3 , 8 , false ); // identifiant ic
        bitstream_write_bits(jpg->stream , jpg->Cr->h , 4 , false ); // Facteur d'échantillonnage (sampling factor) horizontal, de 1 à 4
        bitstream_write_bits(jpg->stream , jpg->Cr->v , 4 , false ); // Facteur d'échantillonnage (sampling factor) vertical, de 1 à 4
        bitstream_write_bits(jpg->stream , 1 , 8 , false ); // Table de quantification iQ associée, de 0 à 3
}

//---------------------------------------------DHT
      
// pour Y_DC
      bitstream_write_bits(jpg->stream , 0xc4 , 16 , true );
      bitstream_write_bits(jpg->stream , 19 + htables_nb_symbols[DC][Y] , 16 , false );// longueur de la section 
      bitstream_write_bits(jpg->stream , 0 , 3 , false );
      bitstream_write_bits(jpg->stream , 0 , 1 , false ); //type (0=DC, 1=AC)
     bitstream_write_bits(jpg->stream , 0 , 4 , false ) ; // indice (0..3, ou erreur)

    //Nombres de symboles avec des codes de longueur 1 à 16. La somme de ces valeurs représente le nombre total de codes et doit être inférieure ou égale à 256.
    
    for(uint32_t i =0 ; i <16 ; i++){
       bitstream_write_bits(jpg->stream , htables_nb_symb_per_lengths[DC][Y][i] , 8 , false );
    }

     // Table contenant les symboles, triés par longueur (cf 2.8.1)
     for(uint32_t i =0 ; i < htables_nb_symbols[DC][Y] ; i++){
       bitstream_write_bits(jpg->stream , htables_symbols[DC][Y][i] , 8 , false );
    }




// pour Y_AC
      bitstream_write_bits(jpg->stream , 0xc4 , 16 , true );
      bitstream_write_bits(jpg->stream , 19 + htables_nb_symbols[AC][Y] , 16 , false );// longueur de la section 
      bitstream_write_bits(jpg->stream , 0 , 3 , false );
      bitstream_write_bits(jpg->stream , 1 , 1 , false ); //type (0=DC, 1=AC)
     bitstream_write_bits(jpg->stream , 0 , 4 , false ) ; // indice (0..3, ou erreur)

    //Nombres de symboles avec des codes de longueur 1 à 16. La somme de ces valeurs représente le nombre total de codes et doit être inférieure ou égale à 256.
    for(uint32_t i =0 ; i <16 ; i++){
       bitstream_write_bits(jpg->stream , htables_nb_symb_per_lengths[AC][Y][i] , 8 , false );
    }

      
    // Table contenant les symboles, triés par longueur (cf 2.8.1)
     for(uint32_t i =0 ; i < htables_nb_symbols[AC][Y] ; i++){
       bitstream_write_bits(jpg->stream , htables_symbols[AC][Y][i] , 8 , false );
    }

if(jpg->nbr_components==3){
// pour CbCr_DC
      bitstream_write_bits(jpg->stream , 0xc4 , 16 , true );
      bitstream_write_bits(jpg->stream , 19 + htables_nb_symbols[DC][Cb] , 16 , false );// longueur de la section 
      bitstream_write_bits(jpg->stream , 0 , 3 , false );
      bitstream_write_bits(jpg->stream , 0 , 1 , false ); //type (0=DC, 1=AC)
     bitstream_write_bits(jpg->stream , 1 , 4 , false ) ; // indice (0..3, ou erreur)
     
     //Nombres de symboles avec des codes de longueur 1 à 16. La somme de ces valeurs représente le nombre total de codes et doit être inférieure ou égale à 256.
    for(uint32_t i =0 ; i <16 ; i++){
       bitstream_write_bits(jpg->stream , htables_nb_symb_per_lengths[DC][Cb][i] , 8 , false );
    }

     // Table contenant les symboles, triés par longueur (cf 2.8.1)
     for(uint32_t i =0 ; i < htables_nb_symbols[DC][Cb] ; i++){
       bitstream_write_bits(jpg->stream , htables_symbols[DC][Cb][i] , 8 , false );
    }

// pour CbCr_AC
      bitstream_write_bits(jpg->stream , 0xc4 , 16 , true );
      bitstream_write_bits(jpg->stream , 19 + htables_nb_symbols[AC][Cb] , 16 , false );// longueur de la section 
      bitstream_write_bits(jpg->stream , 0 , 3 , false );
      bitstream_write_bits(jpg->stream , 1 , 1 , false ); //type (0=DC, 1=AC)
     bitstream_write_bits(jpg->stream , 1 , 4 , false ) ; // indice (0..3, ou erreur)
      

      //Nombres de symboles avec des codes de longueur 1 à 16. La somme de ces valeurs représente le nombre total de codes et doit être inférieure ou égale à 256.
    for(uint32_t i =0 ; i <16 ; i++){
       bitstream_write_bits(jpg->stream , htables_nb_symb_per_lengths[AC][Cb][i] , 8 , false );
    }

     
     
     // Table contenant les symboles, triés par longueur (cf 2.8.1)
     for(uint32_t i =0 ; i < htables_nb_symbols[AC][Cb] ; i++){
       bitstream_write_bits(jpg->stream , htables_symbols[AC][Cb][i] , 8 , false );
    }

}

//---------------------------------------------SOS
    
    bitstream_write_bits(jpg->stream , 0xda , 16 , true );
     bitstream_write_bits(jpg->stream , 2*(jpg->nbr_components)+6 , 16 , false );
    bitstream_write_bits(jpg->stream , jpg->nbr_components , 8 , false );

// pour Y 
        
        bitstream_write_bits(jpg->stream , 1 , 8 , false ); // identifiant ic
        bitstream_write_bits(jpg->stream , 0 , 4 , false ); // 4 bits : indice de la table de Huffman (iH) pour les coefficients DC (iAC/DC=DC)
        bitstream_write_bits(jpg->stream , 0 , 4 , false ); // 4 bits : indice de la table de Huffman (iH) pour les coefficients AC (iAC/DC=AC)

if(jpg->nbr_components==3){

//pour Cb 

        bitstream_write_bits(jpg->stream , 2 , 8 , false ); // identifiant ic
        bitstream_write_bits(jpg->stream , 1 , 4 , false ); // 4 bits : indice de la table de Huffman (iH) pour les coefficients DC (iAC/DC=DC)
        bitstream_write_bits(jpg->stream , 1 , 4 , false ); // 4 bits : indice de la table de Huffman (iH) pour les coefficients AC (iAC/DC=AC)

//pour Cr 

        bitstream_write_bits(jpg->stream , 3 , 8 , false ); // identifiant ic
        bitstream_write_bits(jpg->stream , 1 , 4 , false ); // 4 bits : indice de la table de Huffman (iH) pour les coefficients DC (iAC/DC=DC)
        bitstream_write_bits(jpg->stream , 1 , 4 , false ); // 4 bits : indice de la table de Huffman (iH) pour les coefficients AC (iAC/DC=AC)

}       



    bitstream_write_bits(jpg->stream , 0 , 8 , false );
    bitstream_write_bits(jpg->stream , 63 , 8 , false );
    bitstream_write_bits(jpg->stream , 0 , 4 , false );
    bitstream_write_bits(jpg->stream , 0 , 4 , false );


   





    return;
}

/* Ecrit le footer JPEG (marqueur EOI) dans le fichier de sortie. */
void jpeg_write_footer(struct jpeg *jpg){

    bitstream_flush(jpg->stream);
    bitstream_write_bits(jpg->stream , 0xd9 , 16 , true );
}


/* Ecrit le nom de fichier PPM ppm_filename dans la structure jpeg. */
void jpeg_set_ppm_filename(struct jpeg *jpg,
                           const char *ppm_filename){

    jpg->name_fppm = ppm_filename ; 



                           }

/* Retourne le nom de fichier PPM lu dans la structure jpeg. */
char *jpeg_get_ppm_filename(struct jpeg *jpg){

    return( jpg->name_fppm) ;
}



/*
    Ecrit la hauteur de l'image traitée, en nombre de pixels,
    dans la structure jpeg.
*/
void jpeg_set_image_height(struct jpeg *jpg,
                           uint32_t image_height){

         jpg->hauteur = image_height ; 

                           }

/*
   Retourne la hauteur de l'image traitée, en nombre de pixels,
   lue dans la structure jpeg.
*/
uint32_t jpeg_get_image_height(struct jpeg *jpg){

    return(jpg->hauteur);
}

/*
    Ecrit la largeur de l'image traitée, en nombre de pixels,
    dans la structure jpeg.
*/
void jpeg_set_image_width(struct jpeg *jpg,
                          uint32_t image_width){

       jpg->largeur = image_width ; 


                          }

/*
    Retourne la largeur de l'image traitée, en nombre de pixels,
    lue dans la structure jpeg.
*/
uint32_t jpeg_get_image_width(struct jpeg *jpg){

     return(jpg->largeur);

}



/*
    Ecrit le nombre de composantes de couleur de l'image traitée
    dans la structure jpeg.
*/
void jpeg_set_nb_components(struct jpeg *jpg,
                            uint8_t nb_components){

      jpg->nbr_components = nb_components;

                            }

/*
    Retourne le nombre de composantes de couleur de l'image traitée
    lu dans la structure jpeg.c.
*/
uint8_t jpeg_get_nb_components(struct jpeg *jpg){

    return(jpg->nbr_components);
}



/* Ecrit le nom du fichier de sortie jpeg_filename dans la structure jpeg. */
void jpeg_set_jpeg_filename(struct jpeg *jpg,
                            const char *jpeg_filename){

     jpg->name_fjpeg = jpeg_filename ; 

                            }

/* Retourne le nom du fichier de sortie lu depuis la structure jpeg. */
char *jpeg_get_jpeg_filename(struct jpeg *jpg){

   return(jpg->name_fjpeg);

}


/*
    Ecrit dans la structure jpeg le facteur d'échantillonnage sampling_factor à utiliser pour la composante de couleur cc
    et la direction dir.
*/
void jpeg_set_sampling_factor(struct jpeg *jpg,
                              enum color_component cc,
                              enum direction dir,
                              uint8_t sampling_factor){

if(cc== Y){
    if (dir == H){
        jpg->Y->h = sampling_factor ;
    } 
    else if(dir == V){
        jpg->Y->v = sampling_factor ;
    }
    
}

else if (cc== Cr){
    if (dir == H){
        jpg->Cr->h = sampling_factor ;
    } 
    else if(dir == V){
        jpg->Cr->v = sampling_factor ;
    }
    
}

else if (cc== Cb){
    if (dir == H){
        jpg->Cb->h = sampling_factor ;
    } 
    else if(dir == V){
        jpg->Cb->v = sampling_factor ;
    }
    
}



                              }

/*
    Retourne le facteur d'échantillonnage utilisé pour la
    composante de couleur cc et la direction dir,
    lu dans la structure jpeg.
*/
uint8_t jpeg_get_sampling_factor(struct jpeg *jpg,
                                 enum color_component cc,
                                 enum direction dir){



if(cc== Y){
    if (dir == H){
        return(jpg->Y->h)  ;
    } 
    else if(dir == V){
       return(jpg->Y->v)  ;
    }
    
}

else if (cc== Cr){
    if (dir == H){
        return(jpg->Cr->h)  ;
    } 
    else if(dir == V){
        return(jpg->Cr->v)  ;
    }
    
}

else if (cc== Cb){
    if (dir == H){
        return(jpg->Cb->h)  ;
    } 
    else if(dir == V){
        return(jpg->Cb->v)  ;
    }
    
}

                                 }




/*
    Ecrit dans la structure jpeg la table de Huffman
    huff_table à utiliser pour encoder les données de la
    composante fréquentielle acdc, pour la composante de
    couleur cc.
*/
void jpeg_set_huffman_table(struct jpeg *jpg,
                            enum sample_type acdc,
                            enum color_component cc,
                            struct huff_table *htable){

if(cc== Y){
    if (acdc == AC){
        jpg->Y->htable_ac = htable ;
        
    } 
    else if(acdc == DC){
        jpg->Y->htable_dc = htable ;
        
    } 
    
}

else if (cc== Cr){
    if (acdc == AC){
        jpg->Cr->htable_ac = htable ;
    } 
    else if(acdc == DC){
        jpg->Cr->htable_dc = htable ;
    } 
    
}

else if (cc== Cb){
    if (acdc == AC){
        jpg->Cb->htable_ac = htable ;
    } 
    else if(acdc == DC){
        jpg->Cb->htable_dc = htable ;
    } 
    
}


return  ;


                            }






/*
    Retourne un pointeur vers la table de Huffman utilisée
    pour encoder les données de la composante fréquentielle
    acdc pour la composante de couleur cc,
    lue dans la structure jpeg.
*/
struct huff_table *jpeg_get_huffman_table(struct jpeg *jpg,
                                          enum sample_type acdc,
                                          enum color_component cc){




if(cc== Y){
    if (acdc == AC){
        return(jpg->Y->htable_ac) ;
    } 
    else if(acdc == DC){
        return(jpg->Y->htable_dc);
    } 
    
}

else if (cc== Cr){
    if (acdc == AC){
        return(jpg->Cr->htable_ac);
    } 
    else if(acdc == DC){
        return(jpg->Cr->htable_dc) ;
    } 
    
}

else if (cc== Cb){
    if (acdc == AC){
       return(jpg->Cb->htable_ac) ;
    } 
    else if(acdc == DC){
        return(jpg->Cb->htable_dc) ;
    } 
    
}


                                          }





/*
    Ecrit dans la structure jpeg la table de quantification à utiliser
    pour compresser les coefficients de la composante de couleur cc.
*/
void jpeg_set_quantization_table(struct jpeg *jpg,
                                 enum color_component cc,
                                 uint8_t *qtable){

if(cc== Y){
    jpg->quant_table_Y = qtable ;
    
}

else if (cc== Cr || cc== Cb ){
    jpg->quant_table_CbCr = qtable ;
    
}




                                 }

/*
    Retourne un pointeur vers la table de quantification associée à la
    composante de couleur cc, lue dans a structure jpeg.
*/
uint8_t *jpeg_get_quantization_table(struct jpeg *jpg,
                                     enum color_component cc){


if(cc== Y){
    return(jpg->quant_table_Y ) ;
    
}

else if (cc== Cr || cc== Cb ){
    return(jpg->quant_table_CbCr) ;
    
}

                                       }





/*
    Retourne le bitstream associé au fichier de sortie enregistré
    dans la structure jpeg.
*/
struct bitstream *jpeg_get_bitstream(struct jpeg *jpg)
{
    return(jpg->stream);

}


/* void main(void){
    
    struct jpeg *notre_jpeg= jpeg_create(); 
    
    const char *ppm_filename = "le nom du ppm est ça lol";
    const char *jpeg_filename = "test_david2_jpeg.txt";
     

    jpeg_set_ppm_filename(notre_jpeg,ppm_filename );
    
    jpeg_set_image_height(notre_jpeg , 15);
    jpeg_set_image_width(notre_jpeg , 16);
    jpeg_set_nb_components(notre_jpeg , 3);
    jpeg_set_jpeg_filename(notre_jpeg,jpeg_filename );
    jpeg_set_sampling_factor(notre_jpeg, Y,H,2);
    jpeg_set_sampling_factor(notre_jpeg, Y,V,4);
    jpeg_set_sampling_factor(notre_jpeg, Cr,H,3);
    jpeg_set_sampling_factor(notre_jpeg, Cr,V,6);
    jpeg_set_sampling_factor(notre_jpeg, Cb,H,4);
    jpeg_set_sampling_factor(notre_jpeg, Cb,V,8);
    
    notre_jpeg->file_jpeg = fopen(notre_jpeg->name_fjpeg ,"w");
    fprintf(notre_jpeg->file_jpeg ,"hauteur : %u  et largeur : %u \n",notre_jpeg->hauteur,notre_jpeg->largeur );
    fprintf(notre_jpeg->file_jpeg ,"Y : %u x %u \n",notre_jpeg->Y->h,notre_jpeg->Y->v );
    fprintf(notre_jpeg->file_jpeg ,"Cr : %u x %u \n",notre_jpeg->Cr->h,notre_jpeg->Cr->v );
    fprintf(notre_jpeg->file_jpeg ,"Cb : %u x %u \n",notre_jpeg->Cb->h,notre_jpeg->Cb->v );
    fclose(notre_jpeg->file_jpeg);




} */

