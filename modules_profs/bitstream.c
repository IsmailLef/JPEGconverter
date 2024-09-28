#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>



/*
    Structure trouvé dans " https://stackoverflow.com/questions/47929150/writing-bits-to-file "
*/

struct bitstream {

    FILE *file;
    char buffer;
    uint8_t count;

};


struct bitstream *creat_bitstream( const char *file_name){
    
    struct bitstream *un_bitstream = malloc(sizeof(struct bitstream));
    un_bitstream->file = fopen(file_name, "r");
    un_bitstream->buffer = 0;
    un_bitstream->count = 0;

    return un_bitstream;
}



void bitstream_write_bits(struct bitstream *stream, uint32_t value, uint8_t nb_bits, bool is_marker){

    uint8_t val_bit;
    for (uint16_t i = 0; i<nb_bits ; i++){

        val_bit = value%2;
        value = value >> 1;

        stream->buffer = stream->buffer <<1;
        if (val_bit){ 
            stream->buffer = stream->buffer |1;
        }
        stream->count++;
        if (stream->count == 8){
            fwrite(&(stream->buffer), sizeof(char), 1, stream->file);
            stream->count = 0;
            stream->buffer = 0;
        }
    }
}

void bitstream_flush(struct bitstream *stream){

    stream->buffer = (stream->buffer)<< (8-stream->count);
    fwrite(&(stream->buffer), sizeof(char), 1, stream->file);
    stream->count = 0;
    stream->buffer = 0;

}

void bitstream_destroy(struct bitstream *stream){

    fclose(stream->file);
    free(stream);
}

