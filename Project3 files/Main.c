/* Argyros Konstantinos
AM: 2022202000014
dit20014@go.uop.gr

Pavlos Sygrimis
AM: 2022202000202
dit20202@go.uop.gr */

#include <stdio.h>
#include <stdlib.h>
#include "bmp.h"

struct 
{
    unsigned char red;
    unsigned char green;
    unsigned char blue;
}rgb;

// 3 main parameters (command, input_file, output_file)
int main (int argc, char *argv[])
{
    // argv[0] == 'bip' (programm name)
    // argv[1] == command
    // argv[2] == input_file
    // argv[3] == output_file

    FILE* infile, outfile;
    int x,y;                // array/image dimensions
    rgb** pavlos;           // main array, where we read the image
    rgb** sygrimis;         // dublicate array, changes will be stored here
    int i,j;                // Basic array vars    
    // char* file1 = argv[1], file2 = argv[2];

    // rephrasing real file names (adding .bmp)
    char* file1 = argv[2];
    char* file2 = argv[3];
    char* filetype = ".bmp";

    file1 = strcat(argv[2], filetype);
    file2 = strcat(argv[3], filetype);


    // searching and opening files 
    if(file1 == file2)
    {
        printf("Input file and output file must NOT be the same file\n");
        exit(1);
    }

    infile = fopen(file1, "r");
    if(infile == NULL)
    {
        printf("file named '%s' could not be opened\n", file1);
        fclose(infile);
        exit(1);
    }

    outfile = fopen(file2, "w");
    if(outfile == NULL)
    {
        printf("file named '%s' could not be opened\n", file2);
        fclose(infile);
        fclose(outfile);
        exit(1);
    }

    // allocating memory and reading structs
    bmp_header *h1;
    h1 = (bmp_header*) malloc (sizeof(bmp_header));
    if(h1 == NULL)
    {
        printf("Not enough memory. Please download more ram ffs.\n");
        exit(1);
    }
    readΗeader(infile, h1);

    bmp_info_header *h2;
    h2 = (bmp_info_header*) malloc (sizeof(bmp_info_header));
    if(h2 == NULL)
    {
        printf("Not enough memory. Please download more ram ffs.\n");
        exit(1);
    }
    readInfo(infile, h2);

    // storing arrays' dimensions
    x = h2->biWidth;
    y = h2->biHeight;

    //allocating memory for main array
    pavlos = (rgb**) malloc(sizeof(rgb*) * x);
    if(pavlos == NULL)
    {
        printf("Not enough memory. Please download more ram ffs. Pavlaras is out\n");
        exit(1);
    }
    for(i = 0; i < x; i++)
    {
        pavlos[i] = (rgb*) malloc(sizeof(rgb) * y);
        if(pavlos[i] == NULL)
        {
            printf("Not enough memory. Please download more ram ffs. Pavlaras is out\n");
            exit(1);
        }
    }
    
    // dont forget the Putin
    int extrabytes = 0;
    while((x * 3 + extrabytes) % 4 != 0) extrabytes++;

    // reading image colors
    for(i = x; i >= 0; i--);
    {
        for(j = 0; j < y; j++)
        {
            fread(&(pavlos[i][j].blue), sizeof(pavlos[i][j].blue), 1, infile);
            fread(&(pavlos[i][j].green), sizeof(pavlos[i][j].green), 1, infile);
            fread(&(pavlos[i][j].red), sizeof(pavlos[i][j].red), 1, infile);
        }
        fseek(infile, extrabytes, SEEK_CUR); //skips bytes from patting
    }

    // allocating memory for secondary array
    sygrimis = (rgb**) malloc(sizeof(rgb*) * x);
    if(sygrimis == NULL)
    {
        printf("Not enough memory. Please download more ram ffs. Pavlaras is out\n");
        exit(1);
    }
    for(i = 0; i < x; i++)
    {
        sygrimis[i] = (rgb*) malloc(sizeof(rgb) * y);
        if(sygrimis[i] == NULL)
        {
            printf("Not enough memory. Please download more ram ffs. Pavlaras is out\n");
            exit(1);
        }
    }

    // copying image (main array) to secondary array
    /* for(i = 0; i < x; i++)
    {
        for(j = 0; j < y; y++)
        {
            pavlos[i][j] = sygrimis[i][j];
        }
    } ***COMMENTED BECAUSE IT MIGHT NOT BE NEEDED*** */

    // storing command
    char* command = argv[1];

    // detects and verifies input command
    if(command[0] == '-' && command[1] == 'a')                              // print attributes
    {
        printattributes(h1, h2);
        freearrays(pavlos, sygrimis);
        /* free(pavlos);
        free(sygrimis); */
        exit(0);
    }
    else if(command[0] == '-' && command[1] == 'f'  && command[2] == 'h')   // horizontal flip 
    {
        flip(pavlos, sygrimis, x, y, 'h');
    }
    else if(command[0] == '-' && command[1] == 'f'  && command[2] == 'v')   // vertical flip
    {
        flip(pavlos, sygrimis, x, y, 'v');
    }
    else if(command[0] == '-' && command[1] == 'b')                         // regular grey
    {
        rgrey(pavlos, sygrimis, x, y);
    }
    else if(command[0] == '-' && command[1] == 'b'  && command[2] == 'w')   // artistic grey
    {
        int red, green, blue;
        char* wholecommand = argv[1];
        strtok(wholecommand, _);

        char* color = strtok(NULL, _);

        if(color == "red")
        {
            red = 255;
            green = 0;
            blue = 00;
        }
        else if(color == "lime")
        {
            red = 0;
            green = 255;
            blue = 0;
        }
        else if(color == "blue")
        {
            red = 0;
            green = 0;
            blue = 255;
        }
        else if(color == "yellow")
        {
            red = 255;
            green = 255;
            blue = 0;
        }
        else if(color == "cyan" || color == "aqua")
        {
            red = 0;
            green = 255;
            blue = 255;
        }
        else if(color == "magenta" || color == "fuchsia")
        {
            red = 255;
            green = 0;
            blue = 255;
        }
        else if(color == "maroon")
        {
            red = 128;
            green = 0;
            blue = 0;
        }
        else if(color == "olive")
        {
            red = 128;
            green = 128;
            blue = 0;
        }
        else if(color == "green")
        {
            red = 0;
            green = 128;
            blue = 0;
        }
        else if(color == "purple")
        {
            red = 128;
            green = 0;
            blue = 128;
        }
        else if(color == "teal")
        {
            red = 0;
            green = 128;
            blue = 128;
        }
        else if(color == "navy")
        {
            red = 0;
            green = 0;
            blue = 128;
        }
        else
        {
            printf("Error! Color named '%s' does not exists\n", color);
            exit(1);
        }

        char* number = strtok(NULL, _);
        int percentage = atoi(number);
        if(percentage > 100 || percentage < 0)
        {
            printf("ERROR!\n");
            exit(1);
        }

        agrey(pavlos, sygrimis, x, y, red, green, blue, percentage);
    }
    else if(command[0] == '-' && command[1] == 'p')                         // restrict pallet color selection
    {
        
    }
    else
    {
        printf("Invalid input. Command '%s' does not exist\n", argv[1]);
        freearrays(pavlos, sygrimis, x);
        /* free(pavlos);
        free(sygrimis); */
        exit(1);
    }

    // write output file from secondary array (sygrimis) 


    //free


    return 0;
}
