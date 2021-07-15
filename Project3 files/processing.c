/* Argyros Konstantinos
AM: 2022202000014
dit20014@go.uop.gr

Pavlos Sygrimis
AM: 2022202000202
dit20202@go.uop.gr */

//foo.c

#include "bmp.h"
#include "processing.h"

// #include <math.h>
// prototypes


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
    loading_screen ("dublicating image", i, x);
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
        loading_screen ("flipping image", i, x);
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
        loading_screen ("flipping image", i, x);
    }
}

void agrey (rgbe** mainarray, rgbe** duparray, int x, int y, int red, int green, int blue, int percentage)
{
    int i, j, quantity, count;
    double color_distance;
    unsigned char pred, pgreen, pblue;

    printf("DEBUG: inside agrey\n");


    rgrey(mainarray, duparray, x, y);
    quantity = 0;
    count = 0;

    // Counting pixels of input color
    for(i = 0; i < x; i ++)
    {
        for(j = 0; j < y; j++)
        {
            pred    = mainarray[i][j].red;
            pgreen  = mainarray[i][j].green;
            pblue   = mainarray[i][j].blue;

            color_distance = euclidean_distance(red, green, blue, pred, pgreen, pblue);
            if(color_distance < ACCEPTABLE_COLOR_DISTANCE)
            {
                quantity++;
            }
        }
    }

    for(i = 0; i < x; i ++)
    {
        for(j = 0; j < y; j++)
        {
            pred    = mainarray[i][j].red;
            pgreen  = mainarray[i][j].green;
            pblue   = mainarray[i][j].blue;
            if((color_distance < ACCEPTABLE_COLOR_DISTANCE) && ((count/quantity) * 100 < percentage))
            {
                duparray[i][j].red   = pred; 
                duparray[i][j].green = pgreen;    
                duparray[i][j].blue  = pblue;
                count++;
            }

            if(((count/quantity) * 100 >= percentage))
            {
                break;
            }
            loading_screen ("adding final touches", (count/quantity) * 100, percentage);
        }
    }
    printf("DEBUG: outside agrey\n");
}

double euclidean_distance (int ired, int igreen, int iblue, int pred, int pgreen, int pblue)
{
    int x1, x2, x3;
    double sum;
    x1 = ired + pred;
    x1 = x1 * x1;

    x2 = igreen + pgreen;
    x2 = x2 * x2;

    x3 = iblue + pblue;
    x3 = x3 * x3;

    sum = x1 + x2 + x3;
    // sum = pow(sum, 0.5);
    return sum;
    return 0;
}

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

void free_all_mem (rgbe** mainarray, rgbe** duparray, int x, struct bmp_header *h1, struct bmp_info *h2)
{
    //frees all allocated memmory
    printf("\nDEBUG MESSAGE: Freeing Process started\n");
    freearrays(mainarray, duparray, x);
    free(h1);
    free(h2);
}

// copy-pasted from Project_2 with slight changes
int commandcheck (char* command,char legitcommand[])
{
    lowercaps(command);
    return !(strcmp(command,legitcommand));
}

// copy-pasted from Project_2 with slight changes
char *lowercaps (char *array)
{
    int i;
    for(i = 0; i < 100; i++)
    {
        if(array[i] >= 65 && array[i] <= 90)
        {
            array[i] = array[i] + (97 - 65);
        }
        if(array[i] == '\0')
        {
            break;
        }
    }
    // array[i] = '\0';
    return array;
}

void rotate_up(rgbe **mainarray, rgbe **duparray, int x, int y)
{
    int i, j;
    // printf("DEBUG MESSAGE: inside rotate_up\n");
    for(i = 0; i < x; i++)
    {
        for(j = 0; j < y; j++)
        {
            duparray[i][j].red =    mainarray[(x-1)-i][(y-1)-j].red;
            duparray[i][j].green =  mainarray[(x-1)-i][(y-1)-j].green;
            duparray[i][j].blue =   mainarray[(x-1)-i][(y-1)-j].blue;
            loading_screen ("rotating image", i, x-1);
        }
    }
}

void rotate_left(rgbe **mainarray, rgbe **duparray, int x, int y)
{
    printf("DEBUG FLAAAG4\n");
    int i, j;
    printf("DEBUG MESSAGE: inside rotate_left\n");
    for(i = 0; i < x; i++)
    {
        for(j = 0; j < y; j++)
        {
            duparray[(y-1)-j][i].red =    mainarray[i][j].red;
            duparray[(y-1)-j][i].green =  mainarray[i][j].green;
            duparray[(y-1)-j][i].blue =   mainarray[i][j].blue;
            loading_screen ("rotating image", i, x-1);
        }
    }
}

void rotate_right(rgbe **mainarray, rgbe **duparray, int x, int y)
{
    int i, j;
    printf("DEBUG MESSAGE: inside rotate_right\n");
    for(i = 0; i < x; i++)
    {
        for(j = 0; j < y; j++)
        {
            duparray[j][(x-1)-i].red =    mainarray[i][j].red;
            duparray[j][(x-1)-i].green =  mainarray[i][j].green;
            duparray[j][(x-1)-i].blue =   mainarray[i][j].blue;
            loading_screen ("rotating image", i, x-1);
        }
    }
}

void free_all_mem2 (rgbe** mainarray, rgbe** duparray, int x, int y, struct bmp_header *h1, struct bmp_info *h2)
{
    //frees all allocated memmory
    printf("\nDEBUG MESSAGE: Freeing Process started\n");
    int i, j;
    printf("\nDEBUG MESSAGE: 1\n");

    for(i = 0; i < x; i++)
    {
        free(mainarray[i]);
    }
    free(mainarray);

    printf("\nDEBUG MESSAGE: 2\n");
    for(i = 0; i < y; i++)
    {
        free(duparray[i]);
    }
    free(duparray);

    free(h1);
    free(h2);
}

void loading_screen (char message[], int base, int top)
{
    float completion_percentage;

    printf("\r%s",message);
    completion_percentage = (float) ((base/1.0)/top)*100;
    // printf("DEBUG base == %d\n", base);
    // printf("DEBUG top == %d\n", top);
    // printf("DEBUG percentage == %f\n", completion_percentage);
    if(completion_percentage < 10.0)
    {
        printf("[..........]%.2f%%", completion_percentage);
    }
    else if(completion_percentage < 20.0)
    {
        printf("[#.........]%.2f%%", completion_percentage);
    }
    else if(completion_percentage < 30.0)
    {
        printf("[##........]%.2f%%", completion_percentage);
    }else if(completion_percentage < 40.0)
    {
        printf("[###.......]%.2f%%", completion_percentage);
    }else if(completion_percentage < 50.0)
    {
        printf("[####......]%.2f%%", completion_percentage);
    }else if(completion_percentage < 60.0)
    {
        printf("[#####.....]%.2f%%", completion_percentage);
    }else if(completion_percentage < 70.0)
    {
        printf("[######....]%.2f%%", completion_percentage);
    }else if(completion_percentage < 80.0)
    {
        printf("[#######...]%.2f%%", completion_percentage);
    }else if(completion_percentage < 90.0)
    {
        printf("[########..]%.2f%%", completion_percentage);
    }else if(completion_percentage < 95.0)
    {
        printf("[#########.]%.2f%%", completion_percentage);
    }
    else
    {
        printf("[##########]%.2f%%", completion_percentage);
    }
    delay(5);
}

void delay(int seconds) 
{ 
    int ms;
    // Converting time into ms 
    ms = 1000 * seconds; 
  
    // Storing start time 
    clock_t start_time = clock(); 
  
    // looping till required time is not achieved 
    while (clock() < start_time + ms) 
        ; 
}