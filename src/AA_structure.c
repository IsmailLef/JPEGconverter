
#include <stdint.h>

// Une matrice normale qui contient nb des colonnes et des lignes ainsi que tous les éléments à l'interieur qui sont des nombres

struct matrix {
    int16_t **matrice;
    uint32_t col;
    uint32_t lines;
};

/*  Cette structure reprsente une seul MCU qui peunt contenir plusieurs blocs on sait si la MCU est 2x2, 2x1 , 1x2 ou 1x1 grace à bloc col et bloc line
    si la matrice est 8x8:
                            - blocs pointe vers NULL et matrix contient les éléments du bloc
    sinon:
                            - blocs pointe vers les blocs 8x8 et matrix est NULL
*/

struct MCU_s {
    int16_t bloc_col;
    int16_t bloc_line;
    struct MCU_s **blocs;
    int16_t **matrix;
};


// C'est une matrice des MCUs qui contient le nombre de colonnes et de lignes de MCUs ainsi qu'une matrice qui pointes vers des MCU_s

struct matrix_MCU{
    struct MCU_s **matrice;
    uint32_t MCU_col;
    uint32_t MCU_lines;
};


// STructure qui permet de définire les composants d'un pixel RGB

struct pixel_R{
    int16_t R;
    int16_t G;
    int16_t B;
};

// STructure qui permet de définire les composants d'un pixel YCbCr
struct pixel_Y{
    // on a fait des int car les valeurs peuvent être négatif ( je pense que c'est faux)
    int16_t Y;
    int16_t Cb;
    int16_t Cr;
};


// ################################################################# COULEURS

// struct matrix {
//     struct pixel_R **matrice;
//     uint32_t col;
//     uint32_t lines;
// };

// struct matrix_MCU{
//     struct MCU_s **matrice;
//     uint32_t MCU_col;
//     uint32_t MCU_lines;
// };

// struct pixel_R{
//     int16_t R;
//     int16_t G;
//     int16_t B;
// };

// struct MCU_s {
//     uint8_t bloc_col;
//     uint8_t bloc_line;
//     struct MCU_s **blocs;
//     struct pixel_R **matrix;
// };

// ##################################################################### COULEURS

/*
    C'est une structure qui pointe vers 3 composantes:
        - Une matrice de taille 8 de la composante Y d'un bloc 8x8
        - Une matrice de taille 8 de la composante Cb d'un bloc 8x8
        - Une matrice de taille 8 de la composante Cr d'un bloc 8x8
*/

struct Matrix_YCbCr{
    int16_t **Y;
    int16_t  **Cb;
    int16_t **Cr;
};


/*
    C'est une structure qui a en elle les differents blocs de la MCU ( pointés par l'argument Blocs qui pointe vers des Matrix_YCbCr)
    ces blocs sont sous forme YCbCr et on a aussi ele numéro des blocs ( col x lines) et la forme 

*/

struct MCU_YCbCr {
    struct Matrix_YCbCr *blocs;
    uint8_t col;
    uint8_t lines;
};

/*
    Cette structure contient TOUS les MCUs de l'image dans une Matrice qui symbolise l'emplacement relatif à chaque MCU dans la grille du découpage
    ainsi que le nombre de colonnes et de lignes dans la grille
*/

struct matrix_MCU_YCbCr{
    //Convertit la matrice des MCU_s correspondante à l'image en MCU_s YCbCr
    struct MCU_YCbCr **matrice;
    uint32_t bloc_col;
    uint32_t bloc_line;
};

/*
    Cette structure stocke en elle la valeur de la magnitude et l'indice d'une valeur, ainsi que le nombre de bits minimal sur lequel on peut écrire l'indice en binaire
*/

struct magnitude_s{
     uint16_t magnitude;
     uint16_t indice;
     uint8_t *nb_bits;
};

/*
    C'est pas une matrice, mais un vecteur de vecteur . Le byte_lenght permet de dire le nombre de sous-vecteurs dans le vecteurs, et *vector est l'un des sous-vecteurs
*/

struct vector{
    int16_t **vector;
    uint8_t byte_lenght;
};

/*
    Cette structure sert quand on change la structure des MCUs de matrices en vecteurs, puisque elle permet de stocker les composantes Y, Cb et Cr sous formes 
    de vecteurs dans une structure "vector" expliqué juste avant
*/

struct MCU_vector{
    struct vector vector_Y;
    struct vector vector_Cb;
    struct vector vector_Cr;
};


/*
    Une liste, qui stock en elle les differents composants DC d'une MCU
*/

struct DC_MCU{
    int16_t *Y;
    int16_t *Cb;
    int16_t *Cr;
};