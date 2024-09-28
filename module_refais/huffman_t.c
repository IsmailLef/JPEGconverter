#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>
#include <math.h>

#include "huffman_t.h"

// struct Cell{
//     uint32_t code;
//     uint8_t symbole;
// };

// struct huff_table{
//     uint8_t nb_symbols;
//     //uint8_t *symbols;
//     uint8_t *nb_symb_per_lengths;
//     struct Cell **code_val; // c'est un mélange entre une table de hachage et une matrice ^^
// };


struct huff_table *huffman_table_build(uint8_t *nb_symb_per_lengths, uint8_t *symbols, uint8_t nb_symbols){

    struct huff_table *tab_huffman = malloc(sizeof(struct huff_table));
    tab_huffman->nb_symb_per_lengths = nb_symb_per_lengths;
    // tab_huffman->symbols = symbols;
    tab_huffman->nb_symbols = nb_symbols;
    tab_huffman->code_val = calloc(16, sizeof(struct Cell*));

    uint32_t code = 0;
    uint8_t idx_symb = 0;

    for ( uint8_t depth = 0; depth < 16; depth++){
        
        if (nb_symb_per_lengths[depth] == 0 ) continue;
        
        tab_huffman->code_val[depth] = calloc(nb_symb_per_lengths[depth], sizeof(struct Cell));

        for ( uint8_t nb_symb_depth = 0; nb_symb_depth < nb_symb_per_lengths[depth]; nb_symb_depth++){
            // tab_huffman->codes[depth][nb_symb_depth] = code;
            tab_huffman->code_val[depth][nb_symb_depth].code = code;
            tab_huffman->code_val[depth][nb_symb_depth].symbole = symbols[idx_symb];
            idx_symb++;
            code ++;
        }
        code = code <<1;

    }

    return tab_huffman;

}

void affiche_ht(struct huff_table *ht){

    for ( uint8_t depth = 0; depth<16; depth++){
        printf("######### Longueur :%i\n", depth+1);
        for ( uint16_t idx_depth = 0; idx_depth<ht->nb_symb_per_lengths[depth]; idx_depth++){
            printf("    symbole : %i || code : %i \n", ht->code_val[depth][idx_depth].symbole, ht->code_val[depth][idx_depth].code);
        }
    }

}


uint32_t huffman_table_get_path(struct huff_table *ht, uint8_t value, uint8_t *nb_bits){
    
    for ( uint8_t depth = 0; depth< 16; depth++){
        // printf("longueur %i \n", depth+1);
        if (ht->nb_symb_per_lengths[depth] == 0) continue;
        for (uint8_t idx = 0; idx < ht->nb_symb_per_lengths[depth]; idx++){
            // printf("nombre d'éléments dans cette longueur %i\n",  ht->nb_symb_per_lengths[depth]);
            if (ht->code_val[depth][idx].symbole == value){
                // printf("la valeur est trouvé elle est %i \n", ht->code_val[depth][idx].symbole);
                // printf("à la profondeur : %i  ", depth +1);
                // printf("à l'indice = %i\n", idx);
                // printf(" la profondeur est %i \n", depth+1);
                *nb_bits = depth+1;
                // printf(" la valeur du code est %i\n ",  ht->code_val[depth][idx].code);
                uint8_t val = ht->code_val[depth][idx].code;
                return val;
            } 
        }
    }

}


uint8_t *huffman_table_get_symbols(struct huff_table *ht){

    uint8_t *tab_symbols = calloc(ht->nb_symbols, sizeof(uint8_t));
    uint8_t idx_symb = 0;
    for ( uint8_t depth = 0; depth<16; depth++){
        for ( uint16_t idx_depth = 0; idx_depth<ht->nb_symb_per_lengths[depth]; idx_depth++){
            tab_symbols[idx_symb] = ht->code_val[depth][idx_depth].symbole;
            idx_symb++;
        }
    }
    return tab_symbols;
}


void afficher_liste(uint8_t *tab, uint8_t nb_elem){
    for (uint8_t i =0; i < nb_elem; i++){
        printf(" %i ||", tab[i]);
    }
    printf("\n");
}


uint8_t *huffman_table_get_length_vector(struct huff_table *ht){

    return ht->nb_symb_per_lengths;
}


void huffman_table_destroy(struct huff_table *ht){

    for ( uint8_t depth = 0; depth < 16; depth++){
        free ( ht->code_val[depth]);
    }
    free(ht->code_val);
    free(ht);
}


// int main (){
//     uint8_t nb_symbols = 5;
//     uint8_t symbols[] = {13, 14, 10, 12, 11};
//     uint8_t nb_symbols_per_lenght[] = {0, 3, 2, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
//     uint8_t *nb_bit = malloc(sizeof(uint8_t));

//     struct huff_table *tab_huffman = huffman_table_build(nb_symbols_per_lenght, symbols, nb_symbols);

//     affiche_ht(tab_huffman);
    
//     uint8_t *tab = huffman_table_get_symbols(tab_huffman);
//     afficher_liste(tab, tab_huffman->nb_symbols);

//     uint32_t path = huffman_table_get_path(tab_huffman, 10, nb_bit);
//     printf("le code de 10 est %i, et peut être écrit sur %i bits \n", path, *nb_bit);

//     path = huffman_table_get_path(tab_huffman, 11, nb_bit);
//     printf("le code de 11 est %i, et peut être écrit sur %i bits \n", path, *nb_bit);

//     path = huffman_table_get_path(tab_huffman, 14, nb_bit);
//     printf("le code de 14 est %i, et peut être écrit sur %i bits \n", path, *nb_bit);

//     // printf("%i\n", tab_huffman->code_val[1][0].code);
//     // printf("%i\n", tab_huffman->code_val[1][0].symbole);

//     free(tab);
//     free(nb_bit);
//     huffman_table_destroy(tab_huffman);

// }