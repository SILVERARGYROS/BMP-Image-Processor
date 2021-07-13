/* Argyros Konstantinos
AM: 2022202000014
dit20014@go.uop.gr

Pavlos Sygrimis
AM: 2022202000202
dit20202@go.uop.gr */

//foo.c

#include "bmp.h"

// prototypes

void freearrays (rgbe** pavlos, rgbe** sygrimis, int x)
{
    int i, j;
    for(i = 0; i < x; i++)
    {
        free(pavlos[i]);
    }
    free(pavlos);

    for(i = 0; i < x; i++)
    {
        free(sygrimis[i]);
    }
    free(sygrimis);
}

void printattributes (struct bmp_header *h1,struct bmp_info *h2)
{
    // prints all struct attributes
    printf("DEBUG MESSAGE: successful so far\n");
}

void flip (rgbe** pavlos, rgbe** sygrimis, int x, int y, char lever)
{
    int i, j;
    for(i = 0; i < x; i++)
    {
        for(j = 0; j < y; j++)
        {
            if(lever == 'h')
            {
                pavlos[i][j] = sygrimis[(x-1)-i][j];     
            }
            else if(lever == 'v')
            {
                pavlos[i][j] = sygrimis[i][(y-1)-j];
            }
        }
    }
}

void rgrey (rgbe** pavlos, rgbe** sygrimis, int x, int y)
{
    int sum, i, j;
    for(i = 0; i < x; i++)
    {
        for(j = 0; j < y; j++)
        {
            sum = (pavlos[i][j].red + pavlos[i][j].green + pavlos[i][j].blue)/3;
            sygrimis[i][j].red = sum;
            sygrimis[i][j].green = sum;
            sygrimis[i][j].blue = sum;
        }
    }
}

void agrey (rgbe** pavlos, rgbe** sygrimis, int x, int y, int red, int green, int blue, int percentage)
{
    int i, j;
    rgrey(pavlos, sygrimis, x, y);
    
}