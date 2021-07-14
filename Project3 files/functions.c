/* Argyros Konstantinos
AM: 2022202000014
dit20014@go.uop.gr

Pavlos Sygrimis
AM: 2022202000202
dit20202@go.uop.gr */

//foo.c

#include "bmp.h"

// prototypes

void freearrays (rgbe** mainarray, rgbe** duparray, int x)
{
    int i, j;
    for(i = 0; i < x; i++)
    {
        free(mainarray[i]);
    }
    free(mainarray);

    for(i = 0; i < x; i++)
    {
        free(duparray[i]);
    }
    free(duparray);
}

void printattributes (struct bmp_header *h1,struct bmp_info *h2)
{
    // prints all struct attributes
    printf("\nFILE DETAILS\n");

    printf("Type: %d\n", h1->bfType);
    printf("Size: %d\n", h1->bfSize);
    printf("Reserved1: %d\n", h1->bfReserved1);
    printf("Reserved2: %d\n", h1->bfReserved2);
    printf("OffBits: %d\n", h1->bfOffBits);

    printf("Size: %d\n", h2->bmiHeader.biSize);
    printf("Width: %d\n", h2->bmiHeader.biWidth);
    printf("Height: %d\n", h2->bmiHeader.biHeight);
    printf("Planes: %d\n", h2->bmiHeader.biPlanes);
    printf("BitCount: %d\n", h2->bmiHeader.biBitCount);
    printf("Compression: %d\n", h2->bmiHeader.biCompression);
    printf("SizeImage: %d\n", h2->bmiHeader.biSizeImage);
    printf("XPelsPerMeter: %d\n", h2->bmiHeader.biXPelsPerMeter);
    printf("YPelsPerMeter: %d\n", h2->bmiHeader.biYPelsPerMeter);
    printf("ClrUsed: %d\n", h2->bmiHeader.biClrUsed);
    printf("ClrImportant: %d\n", h2->bmiHeader.biClrImportant);
    printf("DEBUG MESSAGE: printattributes successful\n");
}

void dublicate (rgbe** mainarray, rgbe** duparray, int x, int y)
{
    int i, j;
    for(i = 0; i < x; i++)
    {
        for(j = 0; j < y; j++)
        {
            duparray[i][j] = mainarray[i][j];
        }
    }
}

void flip (rgbe** mainarray, rgbe** duparray, int x, int y, char lever)
{
    // printf("DEBUG MESSAGE: COMMAND DETECTED SUCCESSFULLY\n");
    int i, j;
    for(i = 0; i < x; i++)
    {
        for(j = 0; j < y; j++)
        {
            if(lever == 'h')
            {
                duparray[i][j] = mainarray[(x-1)-i][j];     
            }
            else if(lever == 'v')
            {
                duparray[i][j] = mainarray[i][(y-1)-j];
            }
        }
    }
}

void rgrey (rgbe** mainarray, rgbe** duparray, int x, int y)
{
    int sum, i, j;
    for(i = 0; i < x; i++)
    {
        for(j = 0; j < y; j++)
        {
            sum = (mainarray[i][j].red + mainarray[i][j].green + mainarray[i][j].blue)/3;
            duparray[i][j].red = sum;
            duparray[i][j].green = sum;
            duparray[i][j].blue = sum;
        }
    }
}

void agrey (rgbe** mainarray, rgbe** duparray, int x, int y, int red, int green, int blue, int percentage)
{
    int i, j;
    rgrey(mainarray, duparray, x, y);
    
}