#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdio.h>

// test de l'affichage des arguments
int main_2(int argc, char * argv[]){

    for (uint8_t i = 1; i < argc; i++){
        char * arg = argv[i];
        printf("%s\n", arg);
    }
    return EXIT_SUCCESS;
}


int main_1( ){

    printf(" a = %i\n", 7);
    printf(" b = %i\n", 2);
    float num = 0.299*145.0 + 0.587*200.0 + 0.114*45.0;
    printf(" le résultat a/b en float est : %f\n", num);

    int num_i =0.299*145 + 0.587*200 + 0.114*45;
    printf(" le résultat a/b en float est : %i\n", num_i);

}


int main_3(){

    uint8_t tab[] = {1, 14, 16, 88, 54, 115, 247, 187};
    for (uint8_t i = 0; i < 8; i++){

        printf("la valeur en héxa du nombre %i est : %x \n", tab[i], tab[i]);
    }
}

// code copié depuis le lien suivant https://sebastienguillon.com/test/javascript/convertisseur.html

uint16_t hexa_to_deci ( char* pszHexa ){

    uint32_t ret = 0, t = 0, n = 0;
    const char *c = pszHexa;
    while(*c && (n < 16)) {
        if((*c >= '0') && (*c <= '9')) t = (*c - '0');
        else if((*c >= 'A') && (*c <= 'F')) t = (*c - 'A' + 10);
        else if((*c >= 'a') && (*c <= 'f')) t = (*c - 'a' + 10);
        else break;
        n++; ret *= 16; ret += t; c++;
        if(n >= 8) break;
    }
        return ret;

}

int main(){

    uint16_t ret = 0;
    const char * c;
    printf("la valeur F5A en décimal est : %i \n", hexa_to_deci("F5A"));

}


