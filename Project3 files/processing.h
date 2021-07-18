// our own library choices from main
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <time.h>

// our own structs from main
typedef struct 
{
    unsigned char red;
    unsigned char green;
    unsigned char blue;
}rgbe;


// our own function prototypes from main
void freearrays (rgbe** mainarray, rgbe** duparray, int x);
void printattributes (struct bmp_header *h1,struct  bmp_info *h2);
void flip (rgbe** mainarray, rgbe** duparray, int x, int y, char lever);
void rgrey (rgbe** mainarray, rgbe** duparray, int x, int y);
void agrey (rgbe** mainarray, rgbe** duparray, int x, int y, int red, int green, int blue, int percentage, FILE* fpfile1, FILE* fpfile2);
void dublicate (rgbe** mainarray, rgbe** duparray, int x, int y);
float euclidean_distance (int ired, int igreen, int iblue, int pred, int pgreen, int pblue);
void rotate_up (rgbe **mainarray, rgbe **duparray, int x, int y);
void rotate_left (rgbe **mainarray, rgbe **duparray, int x, int y);
void rotate_right (rgbe **mainarray, rgbe **duparray, int x, int y);
int commandcheck (char* command,char legitcommand[]);
char *lowercaps (char *array);
void loading_screen (char message[], int base, int top, int miliseconds);
void delay(int seconds);
void palette (rgbe** mainarray, rgbe** duparray, int x, int y, int restriction_num);

void quickSort(float* arr, int low, int high, int** pixelcords_array);
int partition (float* arr, int low, int high, int** pixelcords_array);

void free_all_mem (rgbe** mainarray, rgbe** duparray, int x, struct bmp_header *h1, struct bmp_info *h2, char* file1, char* file2);
void free_all_mem2 (rgbe** mainarray, rgbe** duparray, int x, int y, struct bmp_header *h1, struct bmp_info *h2, char* file1, char* file2);

void sort_element (float* distance_array, int** pixelcords_array, int numofele);

int calculating_hue (int red, int green, int blue);


