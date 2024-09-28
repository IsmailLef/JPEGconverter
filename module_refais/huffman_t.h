#ifndef _HUFFMAN_REFAITE_H_
#define _HUFFMAN_REFAITE_H_

#include <stdint.h>

struct Cell{
    uint32_t code;
    uint8_t symbole;
};

struct huff_table{
    uint8_t nb_symbols;
    //uint8_t *symbols;
    uint8_t *nb_symb_per_lengths;
    struct Cell **code_val; // c'est un mélange entre une table de hachage et une matrice ^^
};




extern struct huff_table *huffman_table_build(uint8_t *nb_symb_per_lengths, uint8_t *symbols, uint8_t nb_symbols);

extern uint32_t huffman_table_get_path(struct huff_table *ht, uint8_t value, uint8_t *nb_bits);

extern uint8_t *huffman_table_get_symbols(struct huff_table *ht);

extern uint8_t *huffman_table_get_length_vector(struct huff_table *ht);

extern void huffman_table_destroy(struct huff_table *ht);



#endif  /* _HUFFMAN_REFAITE_H_ */