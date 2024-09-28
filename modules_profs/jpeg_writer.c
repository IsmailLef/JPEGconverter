#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>


/* Type énuméré représentant les composantes de couleur YCbCr. */
enum color_component
{
    Y,
    Cb,
    Cr,
    NB_COLOR_COMPONENTS
};

enum sample_type
{
    DC,
    AC,
    NB_SAMPLE_TYPES
};

enum direction
{
    H,
    V,
    NB_DIRECTIONS
};


struct jpeg{
    char *ppm_filename;
    char *jpeg_filename;
    uint32_t image_width;
    uint32_t image_height;
    uint8_t nb_components;
    struct huff_table **tables_huff; // matrice 2*2 huff_tab*(Y + AcDc)
    uint8_t **sampling_factors; // matrice 3*2 facteurs*

};
