/* Argyros Konstantinos
AM: 2022202000014
dit20014@go.uop.gr

Pavlos Sygrimis
AM: 2022202000202
dit20202@go.uop.gr */

#include "bmp.h"

// 3 main parameters (command, input_file, output_file)
int main (int argc, char *argv[])
{
    // argv[0] == 'bip' (programm name)
    // argv[1] == command
    // argv[2] == input_file
    // argv[3] == output_file (ONLY IF command != "-a")

    FILE *fpfile1, *fpfile2;
    int x,y;                // array/image dimensions
    rgbe** pavlos;           // main array, where we read the image
    rgbe** sygrimis;         // dublicate array, changes will be stored here
    int i,j;                // Basic array vars    
    // char* file1 = argv[1], file2 = argv[2];

    // rephrasing real file names (adding .bmp)
    char* filetype = ".bmp";
    char* file1 = argv[2];

    file1 = strcat(argv[2], ".bmp");

    printf("argv[2] == %s\n", argv[2]);


    printf("file1 == %s\n", file1);
    printf("=======\n");
    // searching and opening files 
    /* if(file1 == file2)
    {
        printf("Input file and output file must NOT be the same file\n");
        exit(1);
    } */

    fpfile1 = fopen(file1, "r");
    if(fpfile1 == NULL)
    {
        printf("file named '%s' could not be opened\n", file1);
        exit(1);
    }


    // allocating memory and reading structs
    struct bmp_header *h1;
    h1 = (struct bmp_header*) malloc (sizeof(struct bmp_header));
    if(h1 == NULL)
    {
        printf("Not enough memory. Please download more ram 1.\n");
        exit(1);
    }
    readHeader(fpfile1, h1);

    struct bmp_info *h2;
    h2 = (struct bmp_info*) malloc (sizeof(struct bmp_info));
    if(h2 == NULL)
    {
        printf("Not enough memory. Please download more ram 2.\n");
        exit(1);
    }
    readInfo(fpfile1, h2);

    // storing arrays' dimensions
    x = h2->bmiHeader.biHeight;
    y = h2->bmiHeader.biWidth;
    printf("x = %d\n", x);
    printf("y = %d\n", y);


    //allocating memory for main array
    pavlos = (rgbe**) malloc(sizeof(rgbe*) * x);
    if(pavlos == NULL)
    {
        printf("Not enough memory. Please download more ram 3.\n");
        exit(1);
    }
    for(i = 0; i < x; i++)
    {
        pavlos[i] = (rgbe*) malloc(sizeof(rgbe) * y);
        if(pavlos[i] == NULL)
        {
            printf("Not enough memory. Please download more ram 4.\n");
            exit(1);
        }
    }
    // dont forget the Putin
    int extrabytes = 0;
    while((y * 3 + extrabytes) % 4 != 0) extrabytes++;
    printf("DEBUG: %d\n", extrabytes);

    // reading image colors
    printf("debug == %d", x);

    for(i = (x-1); i >= 0; i--)
    {
        for(j = 0; j < y; j++)
        {
            printf("DEBUG: good so far i == %d j == %d\n", i, j);
            fread(&(pavlos[i][j].blue), sizeof(unsigned char), 1, fpfile1);
            fread(&(pavlos[i][j].green), sizeof(unsigned char), 1, fpfile1);
            fread(&(pavlos[i][j].red), sizeof(unsigned char), 1, fpfile1);

            // fread(&pavlos[i][j], sizeof(rgbe), 1, fpfile1);
        }
        fseek(fpfile1, extrabytes, SEEK_CUR); //skips bytes from patting
    }

    // allocating memory for secondary array
    sygrimis = (rgbe**) malloc(sizeof(rgbe*) * x);
    if(sygrimis == NULL)
    {
        printf("Not enough memory. Please download more ram ffs. Pavlaras is out\n");
        exit(1);
    }
    for(i = 0; i < x; i++)
    {
        sygrimis[i] = (rgbe*) malloc(sizeof(rgbe) * y);
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
        freearrays(pavlos, sygrimis, x);
        /* free(pavlos);
        free(sygrimis); */
        exit(0);
    }
    else
    {
        // if command != "-a" then we have putput file
        char* file2 = argv[3];
        fpfile2 = fopen(file2, "w");
        if(fpfile2 == NULL)
        {
            printf("file named '%s' could not be opened\n", file2);
            fclose(fpfile1);         //closing already opened files
            exit(1);
        }
        file2 = argv[3];
        printf("argv[3] == %s\n", argv[3]);
        printf("file2 == %s\n", file2);


        if(command[0] == '-' && command[1] == 'f'  && command[2] == 'h')   // horizontal flip 
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

            strtok(wholecommand, "_");
            char* color = strtok(NULL, "_");

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

            char* number = strtok(NULL, "_");
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
    }

    // write output file from secondary array (sygrimis) 


    //free

    printf("DEBUG MESSAGE: programm exiting normally\n");
    return 0;
}
