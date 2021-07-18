OUTPUT: main.o processing.o bmp.o
	gcc main.o processing.o bmp.o -lm -o bip

main.o: main.c 
	gcc -c main.c

processing.o: processing.c processing.h
	gcc -c processing.c

bmp.o: bmp.c bmp.h
	gcc -c bmp.c
