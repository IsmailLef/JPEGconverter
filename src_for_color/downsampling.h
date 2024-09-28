#ifndef _DOWNSAMPLING_H_
#define _DOWNSAMPLING_H_



#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <math.h>
#include "RGB_Couleur.h"



struct fact_echant{
   uint8_t h ;
   uint8_t v ; 

};



extern int8_t restriction( struct fact_echant *fact_1 , struct fact_echant *fact_2 , struct fact_echant *fact_3 );


extern int16_t ** sous_echan_hor_and_vert(int16_t ** Cb_bloc0 , int16_t ** Cb_bloc1 , int16_t ** Cb_bloc2 ,int16_t ** Cb_bloc3);


extern int16_t ** sous_echan_hor(int16_t ** Cb_bloc0 , int16_t ** Cb_bloc1);


extern int16_t ** sous_echan_ver(int16_t ** Cb_bloc0 , int16_t ** Cb_bloc1);


extern void sous_echantillonage(struct MCU_YCbCr *MCU_depart , struct fact_echant *fact_1 , struct fact_echant *fact_2 , struct fact_echant *fact_3 );



#endif  /*_DOWNSAMPLING_H_*/