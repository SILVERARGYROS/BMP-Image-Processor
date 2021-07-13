OUTPUT: main.o functions.o bmp.o
	gcc main.o functions.o bmp.o -o bip

main.o: main.c
	gcc -c main.c

functions.o: functions.c
	gcc -c functions.c

bmp.o: bmp.c bmp.o
	gcc -c bmp.c
