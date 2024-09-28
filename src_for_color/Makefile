CC = clang
LD = gcc

# -O0 désactive les optimisations à la compilation
# C'est utile pour débugger, par contre en "production"
# on active au moins les optimisations de niveau 2 (-O2).
CFLAGS = -Wall -Wextra -std=c99 -O0 -g


all:ppm_generator

rgb : decoupe.o DCT.o PPM_FILE.o quantification.o RGB_Couleur.o RLE.o traitement.o zigzag.o htables_prof_mac.o huffman.o jpeg_writer.o bitstream.o magnitude.o
	$(CC) -o rgb decoupe.o DCT.o PPM_FILE.o quantification.o RGB_Couleur.o RLE.o traitement.o zigzag.o htables_prof_mac.o huffman.o jpeg_writer.o bitstream.o magnitude.o -lm

ppm_generator : write_ppm.o decoupe.o DCT.o downsampling.o PPM_FILE.o quantification.o RGB_Couleur.o RLE.o traitement.o zigzag.o htables_prof_mac.o huffman.o jpeg_writer.o bitstream.o magnitude.o
	$(CC) -o ppm_generator write_ppm.o decoupe.o downsampling.o DCT.o PPM_FILE.o quantification.o RGB_Couleur.o RLE.o traitement.o zigzag.o htables_prof_mac.o huffman.o jpeg_writer.o bitstream.o magnitude.o -lm


write_ppm.o : write_ppm.c
	$(CC) -c write_ppm.c

decoupe.o : decoupe.h
DCT.o : DCT.h
PPM_FILE.o : PPM_FILE.h
quantification.o : quantification.h
RGB_Couleur.o : RGB_Couleur.h
RLE.o : RLE.h
traitement.o : traitement.h
zigzag.o : zigzag.h
magnitude.o : magnitude.h
downsampling.o : downsampling.h
huffman.o : huffman.h
jpeg_writer.o : jpeg_writer.h
bitstream.o : bitstream.h

huffman.h : huffman.c
	$(CC) -c huffman.c

jpeg_writer.h : jpeg_writer.c
	$(CC) -c jpeg_writer.c

bitstream.h : bitstream.c
	$(CC) -c bitstream.c

decoupe.h : decoupe.c
	$(CC) -c decoupe.c

magnitude.h : magnitude.c
	$(CC) -c magnitude.c

DCT.h : DCT.c
	$(CC) -c DCT.c

PPM_FILE.h : PPM_FILE.c
	$(CC) -c PPM_FILE.c

quantification.h : quantification.c
	$(CC) -c quantification.c

RGB_Couleur.h : RGB_Couleur.c
	$(CC) -c RGB_Couleur.c

RLE.h : RLE.c
	$(CC) -c RLE.c

traitement.h : traitement.c
	$(CC) -c traitement.c

zigzag.h : zigzag.c
	$(CC) -c zigzag.c

downsampling.h : downsampling.c
	$(CC) -c downsampling.c

clean:
	rm -rf write_ppm.o decoupe.o DCT.o PPM_FILE.o quantification.o RGB_Couleur.o RLE.o traitement.o zigzag.o magnitude.o downsampling.o
