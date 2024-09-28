#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>


struct Node{
    int8_t  value;
    struct Node * fils_g;
    struct Node *fils_d;
    bool not_used;

};


struct huff_table{
    struct Node *racine;

};

struct huff_table *huffman_table_build(uint8_t *nb_symb_per_lengths, uint8_t *symbols, uint8_t nb_symbols){

    struct huff_table *tab_huffman = malloc(sizeof(struct huff_table));
    tab_huffman->racine = malloc(sizeof(struct Node));
    tab_huffman->racine->value = 0;
    tab_huffman->racine->not_used = true;

    // tab_huffman->racine->fils_d = malloc(sizeof(struct Node));
    // tab_huffman->racine->fils_d->not_used = true;
    // tab_huffman->racine->fils_g = malloc(sizeof(struct Node));
    // tab_huffman->racine->fils_g->not_used = true;
    
    struct Node *noeud_actu;
    int16_t idx_symb = nb_symbols;

    for (int8_t i = 16; i>0; i--){ // on commence le parcours des profondeurs à partir de 16 la plus grande valeur
        struct Node *noeud_actu = tab_huffman->racine;

        if (idx_symb < 0) break; // si on a entré toutes les valeurs y'a pas besoin de continuer 

        uint16_t nb_symbole_depth = nb_symb_per_lengths[i];  // représente le nombre de symboles de la profondeurs i
        while ( nb_symbole_depth > 0){ // tant qu'on a encore des symboles appartenant à  la profondeur actuelle de l'arbre on continue
            for (uint8_t depth=0; depth < i; depth++ ){ // on doit atteindre la profondeur recherché en commençant du plus gauche
            /* les plus profonds sont les plus droite, on fait donc le test si d'abord il y'a un noeud droit,
               sinon on l'alloue et fix la valeur "not_used" à true, car il ne contient pas encore de valeur */
                if (noeud_actu->fils_d == NULL){ 
                    noeud_actu->fils_d = malloc(sizeof(struct Node));
                    noeud_actu = noeud_actu->fils_d;
                    noeud_actu->not_used = true;
                }else if (noeud_actu->fils_d->not_used ){ // s'il existe on regarde s'il est déja utilisé ou pas, s'il est pas utilisé on passe à lui
                    noeud_actu = noeud_actu->fils_d;
                }else if ( noeud_actu->fils_g == NULL){ // si le noeud droit était utilisé, on fait la même procédure mais mmnt à gauche
                    noeud_actu->fils_g = malloc(sizeof(struct Node));
                    noeud_actu = noeud_actu->fils_g;
                    noeud_actu->not_used = true;
                } else{
                    noeud_actu = noeud_actu->fils_g;
                }
                // apres for normalement on atteint le noeud où on va écrire la valeur
            }

            noeud_actu->value = symbols[idx_symb]; // on remplie le noeud par sa valeur
            noeud_actu->not_used = false;   // le noeud est utilisé donc not_used == false
            idx_symb --; // on passe au symbole d'avant
        }
    }

    return tab_huffman;
}