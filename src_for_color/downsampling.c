
#include <stdlib.h> 
#include <stdint.h> 
#include <stdio.h> 
#include <string.h> 
#include <ctype.h> 
#include <math.h> 
#include "downsampling.h"


//-------------------------------------------------------------------------------------------------------------------------------------------- Les restrictions 

int8_t restriction( struct fact_echant *fact_1 , struct fact_echant *fact_2 , struct fact_echant *fact_3 ){

  uint8_t h1 = fact_1 ->h ;
  uint8_t v1 = fact_1 ->v ;
  uint8_t h2 = fact_2 ->h ;
  uint8_t v2 = fact_2 ->v ;
  uint8_t h3 = fact_3 ->h ;
  uint8_t v3 = fact_3 ->v ;





//La somme des produits hi×vi doit être inférieure ou égale à 10 ;
   
   if((h1*v1)+(h2*v2)+(h3*v3) > 10){
       return 1 ;
   }

// Les facteurs d'échantillonnage des chrominances doivent diviser parfaitement ceux de la luminance.
   
    else if((h1)%(h2) !=0 || (h1)%(h3) !=0 || (v1)%(v2) !=0  || (v1)%(v3) !=0 ){
              return 1 ;
    }

//La valeur de chaque facteur h ou v doit être comprise entre 1 et 4

   else if((h1)>4 || (v1)>4 || (h2)>4 || (v2)>4 || (h3)>4 || (v3)>4){
       return 1 ; 
   }

   else if((h1)<=0 || (v1)<=0 || (h2)<=0 || (v2)<=0 || (h3)<=0 || (v3)<=0){
       return 1 ; 
   }



return 0 ; 

}











//----------------------------------------------------------------------------------------------- Echantillonage vertical et horizontale 

int16_t ** sous_echan_hor_and_vert(int16_t ** Cb_bloc0 , int16_t ** Cb_bloc1 , int16_t ** Cb_bloc2 ,int16_t ** Cb_bloc3){

    //CREATION CB (ou CR)

   int16_t **CB = calloc(8,sizeof(int16_t*));
   for(uint32_t i = 0 ; i<8 ; i++){
      CB[i]=calloc(8,sizeof(int16_t));
      for(uint32_t j = 0 ; j < 8 ; j++ ){
         if (i<4 && j < 4){
            int16_t moyenne = ( Cb_bloc0[2*i][2*j] + Cb_bloc0[2*i][2*j+1] + Cb_bloc0[2*i+1][2*j] + Cb_bloc0[2*i+1][2*j+1] )*(0.25) ;
            CB[i][j]=moyenne;
         } 
         else if (i<4 && j >= 4){
            uint32_t k=j-4;
            int16_t moyenne = ( Cb_bloc1[2*i][2*k] + Cb_bloc1[2*i][2*k+1] + Cb_bloc1[2*i+1][2*k] + Cb_bloc1[2*i+1][2*k+1] )*(0.25) ;
            CB[i][j]=moyenne ; 
         }    
         else if (i>=4 && j < 4){
            uint32_t k=i-4;
            int16_t moyenne = ( Cb_bloc2[2*k][2*j] + Cb_bloc2[2*k][2*j+1] + Cb_bloc2[2*k+1][2*j] + Cb_bloc2[2*k+1][2*j+1] )*(0.25) ;
            CB[i][j]=moyenne ;
         }
         else if (i>=4 && j >= 4){
            uint32_t  k1=i-4;
            uint32_t  k2=j-4 ;
            int16_t moyenne = ( Cb_bloc3[2*(k1)][2*(k2)] + Cb_bloc3[2*(k1)][2*(k2)+1] + Cb_bloc3[2*(k1)+1][2*(k2)] + Cb_bloc3[2*(k1)+1][2*(k2)+1] )*(0.25) ;
            CB[i][j]=moyenne ; 
         }
      }
   }

   return(CB);
}





//----------------------------------------------------------------------------------------------------------------  SOus echantillonage horizontale  




int16_t ** sous_echan_hor(int16_t ** Cb_bloc0 , int16_t ** Cb_bloc1)
{
    //CREATION CB 

   int16_t **CB = calloc(8,sizeof(int16_t*));
   for(uint32_t i = 0 ; i<8 ; i++){
      CB[i]=calloc(8,sizeof(int16_t));
      for(uint32_t j = 0 ; j < 8 ; j++ ){
         if (j < 4){
            int16_t moyenne = ( Cb_bloc0[i][2*j] + Cb_bloc0[i][2*j+1] )*(0.5) ;
            CB[i][j]=moyenne ; 
         } 
         else{
              uint32_t k=j-4;
            int16_t moyenne = ( Cb_bloc1[i][2*k] + Cb_bloc1[i][2*k+1] )*(0.5) ;
            CB[i][j]=moyenne ; 
         }
      }
   }
   return(CB) ;
}



//----------------------------------------------------------------------------------------------------------------  SOus echantillonage verticale




int16_t ** sous_echan_ver(int16_t ** Cb_bloc0 , int16_t ** Cb_bloc1){

    //CREATION CB 

   int16_t **CB = calloc(8,sizeof(int16_t*));
   for(uint32_t i = 0 ; i<8 ; i++){
      CB[i]=calloc(8,sizeof(int16_t));
      for(uint32_t j = 0 ; j < 8 ; j++ ){
         if (i < 4){
            int16_t moyenne = ( Cb_bloc0[2*i][j] + Cb_bloc0[2*i+1][j] )*(0.5) ;
            CB[i][j]=moyenne ; 
         } 
         else{
            uint32_t k=i-4;
            int16_t moyenne = ( Cb_bloc1[2*k][j] + Cb_bloc1[2*k+1][j] )*(0.5) ;
            CB[i][j]=moyenne ; 
           }
      }
   }
   return(CB);
}















//--------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------Fonction principale 




void sous_echantillonage(struct MCU_YCbCr *MCU_depart , struct fact_echant *fact_1 , struct fact_echant *fact_2 , struct fact_echant *fact_3 ){

     
// Gerer le respect des restrictions
   
int8_t error = restriction(fact_1 ,fact_2, fact_3);
if (error== 1){
    printf("Les facteurs ne respectent pas les restrictions \n");
    return ; 
}




// Voir si l echantillonage demandé peut etre réalisé ( par exemple : une matrice de bloc 2*1 , ne peut recevoir un echantillonage verticale )

if(((MCU_depart->col) != (fact_1 ->h) ) || ((MCU_depart->lines) != (fact_1 ->v) ) ){
    
    printf(" Cet echantillonage n'est pas possible pour cette MCU  \n ");
    return ;

}






// definition pour faciliter l ecriture 

  uint8_t h1 = fact_1 ->h ;
  uint8_t v1 = fact_1 ->v ;
  uint8_t h2 = fact_2 ->h ;
  uint8_t v2 = fact_2 ->v ;
  uint8_t h3 = fact_3 ->h ;
  uint8_t v3 = fact_3 ->v ;


// BLOC 

struct Matrix_YCbCr bloc0= MCU_depart->blocs[0];
struct Matrix_YCbCr bloc1;
struct Matrix_YCbCr bloc2;
struct Matrix_YCbCr bloc3 ; 
// definir les autres blocs en fonction du nombre de blocs 

//_________ Pour 2

if((MCU_depart->lines )* (MCU_depart->col) ==2 ){
 bloc1= MCU_depart->blocs[1];
}

//_________ Pour 3

else if((MCU_depart->lines )* (MCU_depart->col) ==3 ){
   bloc1= MCU_depart->blocs[1];
 bloc2= MCU_depart->blocs[2];
}

//_________ Pour 4

else if((MCU_depart->lines )* (MCU_depart->col) ==4 ){
 bloc1= MCU_depart->blocs[1];
 bloc2= MCU_depart->blocs[2];  
 bloc3= MCU_depart->blocs[3];
}




//CB DES BLOCS 

int16_t ** Cb_bloc0 = bloc0.Cb ; 
int16_t ** Cb_bloc1 ;
int16_t ** Cb_bloc2 ;
int16_t ** Cb_bloc3 ;

// definir les CB des autres blocs en fonction de l existence 

//_________ Pour 2

if((MCU_depart->lines )* (MCU_depart->col) ==2 ){
 Cb_bloc1 = bloc1.Cb ;
}

//_________ Pour 3 ( cas particulier : 3*1 , 3*1 , 3*1 )

else if((MCU_depart->lines )* (MCU_depart->col) ==3 ){
   Cb_bloc1 = bloc1.Cb ;
 Cb_bloc2 = bloc2.Cb ;
}

//_________ Pour 4

else if((MCU_depart->lines )* (MCU_depart->col) ==4 ){
  Cb_bloc1 = bloc1.Cb ;
 Cb_bloc2 = bloc2.Cb ; 
 Cb_bloc3 = bloc3.Cb ;
}


// CR DES BLOCS

int16_t ** Cr_bloc0 = bloc0.Cr ; 
int16_t ** Cr_bloc1 ;
int16_t ** Cr_bloc2 ;
int16_t ** Cr_bloc3 ; 

// definir les CR des autres blocs en fonction de l existence 

//_________ Pour 2

if((MCU_depart->lines )* (MCU_depart->col) ==2 ){
 Cr_bloc1 = bloc1.Cr ;
}

//_________ Pour 3

else if((MCU_depart->lines )* (MCU_depart->col) ==3 ){
   Cr_bloc1 = bloc1.Cr ;
 Cr_bloc2 = bloc2.Cr ;
}

//_________ Pour 4

else if((MCU_depart->lines )* (MCU_depart->col) ==4 ){
   Cr_bloc1 = bloc1.Cr ;
   Cr_bloc2 = bloc2.Cr ; 
   Cr_bloc3 = bloc3.Cr ;
}







// Debut des echantillonages 


// 1---------- Pas de sous echantillonage  


if (((h1)*(v1) == (h2)*(v2)) && ((h2)*(v2)==(h3)*(v3) )){
    
   // printf(" Sous echantillonage 4:4:4   \n ");
 return ; 

}









// 2----------Sous echantillonage horizontale  ( Possibilité : 2*1,1*1,1*1  et 2*2,1*2,1*2    ) ( voir le cas 4*1 , 2*1 , 2*1 )



else if ((( (h1)*(0.5)) == (h2)) && ((h2)==(h3))  && ((v1)==(v2) && ((v2)==(v3)))){

   // Creer matrice CB1 (et CB2 si on a 2 lignes )
   int16_t **CB1 = sous_echan_hor(Cb_bloc0 ,Cb_bloc1);
   int16_t **CB2 = NULL ;
   // si on a 2 lignes ....  ( voir le cas 4*1 , 2*1 , 2*1 ) 
   if(MCU_depart -> lines ==2){
   CB2 = sous_echan_hor(Cb_bloc2 ,Cb_bloc3);
   }
  
   // faire pointer  
   MCU_depart->blocs[0].Cb=CB1 ; 
   MCU_depart->blocs[1].Cb=CB2 ; 
   // si on a deux lignes ....
   if(MCU_depart->lines ==2 ){
   MCU_depart->blocs[2].Cb=CB2 ;
   MCU_depart->blocs[3].Cb=CB2 ;
   }



// Creer matrice CR1 et CR2 ( vu qu on a 4 bloc )
     int16_t **CR1 = sous_echan_hor(Cr_bloc0 ,Cr_bloc1);
     int16_t **CR2 = NULL ;
     // si on a deux lignes .... ( voir le cas 4*1 , 2*1 , 2*1 )
     if(MCU_depart->lines ==2 ){
     CR2 = sous_echan_hor(Cr_bloc2 ,Cr_bloc3);
     }


    // pointer 

   MCU_depart->blocs[0].Cr=CR1 ; 
   MCU_depart->blocs[1].Cr=CR2 ; 
   // si on a deux lignes ....
   if(MCU_depart->lines ==2 ){
   MCU_depart->blocs[2].Cr=CR2 ;
   MCU_depart->blocs[3].Cr=CR2 ;
   }
  //printf(" Sous echantillonage horizontale réalisé  \n ");
   return ; 

}


// 3----------Sous echantillonage verticale  ( Possibilité : 1*2,1*1,1*1  et 2*2,2*1,2*1    ) ( voir le cas particulier  1*4 , 1*2 , 1*2 )



else if ((( (v1)*(0.5)) == (v2)) && ((v2)==(v3))  && ((h1)==(h2)) && ((h2)==(h3))){

// Creer matrice CB1 (et CB2 si on a 2 colonnes )
     
    int16_t **CB1 = NULL ;
    int16_t **CB2 = NULL ; 


     // Si on a qu une colonne 
      if(MCU_depart -> col ==1){
     CB1 = sous_echan_ver(Cb_bloc0 ,Cb_bloc1);
     
     } 

     // si on a deux colonnes ....
     else if(MCU_depart -> col ==2){
     CB1= sous_echan_ver(Cb_bloc0 ,Cb_bloc2);
     CB2 = sous_echan_ver(Cb_bloc1 ,Cb_bloc3);
     }


   //( voir le cas particulier  1*4 , 1*2 , 1*2 )

  
  // faire pointer  

  // Si on a qu une colonne 
  if(MCU_depart -> col ==1){
   MCU_depart->blocs[0].Cb=CB1 ; 
   MCU_depart->blocs[1].Cb=CB1 ;
  } 
   // si on a deux colonnes ....
   else if(MCU_depart->col ==2 ){
   MCU_depart->blocs[0].Cb=CB1 ; 
   MCU_depart->blocs[2].Cb=CB1 ;  
   MCU_depart->blocs[1].Cb=CB2 ;
   MCU_depart->blocs[3].Cb=CB2 ;
   };



// Creer matrice CR1 (et CR2 si on a 2 colonnes )
      
     int16_t **CR1 = NULL ;
    int16_t **CR2 = NULL ; 



     // Si on a qu une colonne 

     if(MCU_depart -> col ==1){
     CR1 = sous_echan_ver(Cr_bloc0 ,Cr_bloc1);
     
     } 

     // si on a deux colonnes ....
     else if(MCU_depart -> col ==2){
     CR1 = sous_echan_ver(Cr_bloc0 ,Cr_bloc2);
     CR2= sous_echan_ver(Cr_bloc1 ,Cr_bloc3);
     };
  
  // faire pointer  

  // Si on a qu une colonne 
  if(MCU_depart -> col ==1){
   MCU_depart->blocs[0].Cr=  CR1 ; 
   MCU_depart->blocs[1].Cr=CR1 ;
  } 
   // si on a deux colonnes ....
   else if(MCU_depart->col ==2 ){
   MCU_depart->blocs[0].Cr=CR1 ; 
   MCU_depart->blocs[2].Cr=CR1 ;  
   MCU_depart->blocs[1].Cr=CR2 ;
   MCU_depart->blocs[3].Cr=CR2 ;
   };

//printf(" Sous echantillonage verticale réalisé  \n ");
 return ; 

}





//4----------Sous echantionnage vertical et horizontal  ( le seul cas possible est le 2*2 , 1*1 , 1*1    )




else if ( ((h1)==2) && ((v1)==2) && ( ((h1)*(v1)*(0.25))== (h2)*(v2) ) && ((h2)*(v2) == (h3)*(v3)) ){

    // Creer matrice CB
     int16_t **CB = sous_echan_hor_and_vert(Cb_bloc0 ,Cb_bloc1 ,Cb_bloc2 , Cb_bloc3);


    //pointer  
    MCU_depart->blocs[0].Cb=CB ; 
   MCU_depart->blocs[1].Cb=CB ; 
   MCU_depart->blocs[2].Cb=CB ;
   MCU_depart->blocs[3].Cb=CB ;




     // Creer matrice CR
     int16_t **CR = sous_echan_hor_and_vert(Cr_bloc0 ,Cr_bloc1 ,Cr_bloc2 , Cr_bloc3);

     //pointer  

   MCU_depart->blocs[0].Cr=CR ; 
   MCU_depart->blocs[1].Cr=CR ; 
   MCU_depart->blocs[2].Cr=CR ;
   MCU_depart->blocs[3].Cr=CR ;
    

   // printf(" Sous echantillonage horizontale et  verticale réalisé  \n ");
    return ; 


}

// 5--------------------------------------- Rien a pu etre fait 

else{
    printf(" rien a pu etre fait  \n ") ; 
    return ; 
}


//------------------------------------------------------------------------------------------------------------------- FIN

}





//-------------------------------------Test


