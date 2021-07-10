//foo.c

#include "bmp.h"

// prototypes
void freearrays (rgb** pavlos, rgb** sygrimis, int x)
{
    for(i = 0; i < x; i++)
    {
        free(pavlos[i]);
    }
    free(pavlos)

    for(i = 0; i < x; i++)
    {
        free(sygrimis[i]);
    }
    free(sygrimis)
}

void printattributes (bmp_header *h1, bmp_info_header *h2)
{
    // prints all struct attributes

}

void flip (rgb** pavlos, rgb** sygrimis, int x, int y, char lever)
{
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

void rgrey (rgb** pavlos, rgb** sygrimis, int x, int y)
{
    int sum;
    for(i = 0; i < x; i++)
    {
        for(j = 0; j < y; j++)
        {
            sum = (pavlos[i][j].red + pavlos[i][j].green + pavlos[i][j].blue)/3
            sygrimis[i][j].red = sum;
            sygrimis[i][j].green = sum;
            sygrimis[i][j].blue = sum;
        }
    }
}

void agrey (rgb** pavlos, rgb** sygrimis, int x, int y, int red, int green, int blue, int percentage)
{
    rgrey(pavlos, sygrimis, x, y);
    
}