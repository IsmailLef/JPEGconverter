CC = gcc
LD = gcc

# -O0 désactive les optimisations à la compilation
# C'est utile pour débugger, par contre en "production"
# on active au moins les optimisations de niveau 2 (-O2).
CFLAGS = -Wall -Wextra -std=c99 -O0 -g
LDFLAGS =


all:img_generator

img_generator : write_img.o decoupe_pgm.o DCT.o PGM_FILE.o quantification.o RGB.o RLE.o traitement.o zigzag.o ../obj/htables.o huffman.o jpeg_writer.o bitstream.o magnitude.o
	gcc -o ppm2jpeg_gray write_img.o decoupe_pgm.o DCT.o PGM_FILE.o quantification.o RGB.o RLE.o traitement.o zigzag.o ../obj/htables.o huffman.o jpeg_writer.o bitstream.o magnitude.o -lm $(LDFLAGS)


write_img.o : write_img.c
	gcc -c write_img.c $(LDFLAGS)

decoupe_pgm.o : decoupe_pgm.h
DCT.o : DCT.h
PGM_FILE.o : PGM_FILE.h
quantification.o : quantification.h
RGB.o : RGB.h
RLE.o : RLE.h
traitement.o : traitement.h
zigzag.o : zigzag.h
magnitude.o : magnitude.h
bitstream.o : bitstream.h
jpeg_writer.o : jpeg_writer.h
huffman.o : huffman.h

decoupe_pgm.h : decoupe_pgm.c
	gcc -c decoupe_pgm.c $(LDFLAGS)

magnitude.h : magnitude.c
	gcc -c magnitude.c $(LDFLAGS)

DCT.h : DCT.c
	gcc -c DCT.c $(LDFLAGS)

PGM_FILE.h : PGM_FILE.c
	gcc -c PGM_FILE.c $(LDFLAGS)

quantification.h : quantification.c
	gcc -c quantification.c $(LDFLAGS)

RGB.h : RGB.c
	gcc -c RGB.c $(LDFLAGS)

RLE.h : RLE.c
	gcc -c RLE.c $(LDFLAGS)

traitement.h : traitement.c
	gcc -c traitement.c $(LDFLAGS)

zigzag.h : zigzag.c
	gcc -c zigzag.c $(LDFLAGS)


huffman.h : huffman.c
	gcc -c huffman.c $(LDFLAGS)

jpeg_writer.h : jpeg_writer.c
	gcc -c jpeg_writer.c $(LDFLAGS)

bitstream.h : bitstream.c
	gcc -c bitstream.c $(LDFLAGS)


clean:
	rm -rf *.o
