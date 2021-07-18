/* Argyros Konstantinos
AM: 2022202000014
dit20014@go.uop.gr

Pavlos Sygrimis
AM: 2022202000202
dit20202@go.uop.gr */

#include "bmp.h"
#include "processing.h"

// 3 main parameters (command, input_file, output_file)
int main (int argc, char *argv[])
{
    // argv[0] == 'bip' (programm name)
    // argv[1] == command
    // argv[2] == input_file
    // argv[3] == output_file (ONLY IF command != "-a")

    if(argc == 1)
    {
        printf("Warning! Programm run without entering paramenters. Task terminated.\n");
        exit(0);
    }

    FILE *fpfile1, *fpfile2; // file pointers
    int x,y;                 // array/image dimensions
    rgbe** mainarray;        // main array, where we read the image
    rgbe** duparray;         // dublicate array, where changes will be stored
    int i, j, z;             // Basic array variables    
    char* file1,* file2;     // file names

    // searching and opening files 
    file1 = strdup(argv[2]);
    strcat(file1, ".bmp");  // rephrasing real file name (adding .bmp)


    // debugging block
    /* printf("DEBUG: argv[2] == %s\n", argv[2]);
    printf("DEBUG: file1 == %s\n", file1);
    printf("=======\n"); */


    fpfile1 = fopen(file1, "r");
    if(fpfile1 == NULL)
    {
        printf("file named '%s' could not be opened\n", file1);
        exit(1);
    }
    // printf("DEBUG MESSAGE: argc == %d\n", argc);
    if(argc > 3)
    {   
        file2 = strdup(argv[3]);
        strcat(file2, ".bmp");  // rephrasing real file name (adding .bmp)

        // comparing file names
        if(!strcmp(file1, file2))
        {
            printf("Input file and output file must NOT be the same file.\n");
            fclose(fpfile1);
            exit(1);
        }

        // checking if file2 already exists ad ask permission to overwrite
        // printf("DEBUG before fpfile2\n");

        if(fpfile2 = fopen(file2, "r"))
        {
            int answer;
            printf("file named '%s' already exists. Are you sure you want to overwrite?? (0 == yes || 1 == no)\n>", file2);
            do
            {
                scanf("%d", &answer);
                if(answer != 0 && answer!= 1)
                {
                    printf("Invalid input. Please try again\n>");
                }
            }while(answer != 0 && answer!= 1);
            if(answer)
            {
                fclose(fpfile1);
                fclose(fpfile2);
                printf("Exiting Programm\n");
                exit(0);
            }
        }
        
        /* printf("DEBUG: argv[3] == %s\n", argv[3]);
        printf("DEBUG: DEBUG: file2 == %s\n", file2); */
        fpfile2 = fopen(file2, "w+");
        if(fpfile2 == NULL)
        {
            printf("file named '%s' could not be opened\n", file2);
            fclose(fpfile1);         //closing already opened files
            exit(1);
        }
    }
        // printf("DEBUG before malloc 1\n");

    // allocating memory and reading structs
    struct bmp_header *h1;
    h1 = (struct bmp_header*) malloc (sizeof(struct bmp_header));
    if(h1 == NULL)
    {
        printf("Not enough memory. Please download more ram 1.\n");
        fclose(fpfile1);
        fclose(fpfile2);
        exit(1);
    }
    readHeader(fpfile1, h1);
    
    // printf("DEBUG before malloc 2\n");
    struct bmp_info *h2;
    h2 = (struct bmp_info*) malloc (sizeof(struct bmp_info));
    if(h2 == NULL)
    {
        printf("Not enough memory. Please download more ram 2.\n");
        fclose(fpfile1);
        fclose(fpfile2);
        exit(1);
    }
    readInfo(fpfile1, h2);

    // storing arrays' dimensions
    x = h2->bmiHeader.biHeight;
    y = h2->bmiHeader.biWidth;
    // printf("x = %d\n", x);
    // printf("y = %d\n", y);
    // printf("DEBUG\n");
    // printf("DEBUG before malloc 3\n");
 
    //allocating memory for main array
    mainarray = (rgbe **) malloc(sizeof(rgbe *) * x);
    if(mainarray == NULL)
    {
        printf("Not enough memory. Please download more ram 3.\n");
        fclose(fpfile1);
        fclose(fpfile2);
        exit(1);
    }
    for(i = 0; i < x; i++)
    {
        mainarray[i] = (rgbe *) malloc(sizeof(rgbe) * y);
        if(mainarray[i] == NULL)
        {
            printf("Not enough memory. Please download more ram 4.\n");
            fclose(fpfile1);
            fclose(fpfile2);
            exit(1);
        }
    }
    // printf("DEBUG before malloc 4\n");


    // calculating padding
    int extrabytes = 0;
    while((y * 3 + extrabytes) % 4 != 0)
    {
        extrabytes++;
    }

    // debugging block
    /* printf("DEBUG: extrabytes == %d\n", extrabytes);
    printf("DEBUG x == %d", x);
    printf("DEBUG before animation\n");
    printf("\n"); */

    // reading image colors
    for(i = (x-1); i >= 0; i--)
    {
        for(j = 0; j < y; j++)
        {
            // printf("DEBUG: good so far i == %d j == %d\n", i, j);
            fread(&(mainarray[i][j].blue), sizeof(unsigned char), 1, fpfile1);
            fread(&(mainarray[i][j].green), sizeof(unsigned char), 1, fpfile1);
            fread(&(mainarray[i][j].red), sizeof(unsigned char), 1, fpfile1);
            // printf("DEBUG MESSAGE: blue:%d green:%d red:%d\n",mainarray[i][j].blue, mainarray[i][j].green, mainarray[i][j].red);
        }
        loading_screen ("loading image",(x-1)-i, x-1, 3);
        fseek(fpfile1, extrabytes, SEEK_CUR); //skips bytes from padding
    }
    /* printf("DEBUG: good so far (read 1st)\n");
    printf("DEBUG: DEBUG1.5.5.5.5.5: file2 == %s\n", file2); */

    // allocating memory for secondary array
    duparray = (rgbe**) malloc(sizeof(rgbe*) * x);
    if(duparray == NULL)
    {
        printf("Not enough memory. Please download more ram.\n");
        fclose(fpfile1);
        fclose(fpfile2);
        exit(1);
    }
    for(i = 0; i < x; i++)
    {
        duparray[i] = (rgbe*) malloc(sizeof(rgbe) * y);
        if(duparray[i] == NULL)
        {
            printf("Not enough memory. Please download more ram ffs.\n");
            fclose(fpfile1);
            fclose(fpfile2);
            exit(1);
        }
    }
    // printf("DEBUG: DEBUG2: file2 == %s\n", file2);

    // detects and verifies input command
    char* command   = strdup(argv[1]);  // storing command

    if(commandcheck(command, "-a"))                              
    {
        // prints attributes
        printattributes(h1, h2);
        for(i = 0; i < x; i++)
        {
            free(mainarray[i]);
        }
        free(mainarray);
        fclose(fpfile1);
        printf("\nProcess successful\n");
        exit(0);
        // printf("DEBUG MESSAGE: outputcheck == %d\n", outputcheck);
    }
    else if(commandcheck(command, "-fh"))   
    {
        // X-axis flip 
        flip(mainarray, duparray, x, y, 'h');
    }
    else if(commandcheck(command, "-fv"))   
    {
        // Y-axis flip
        flip(mainarray, duparray, x, y, 'v');
    }
    else if(commandcheck(strtok(command, "_"), "-bw"))   
    {
        // artistic grey
        int red, green, blue;
        char* wholecommand = argv[1];
        char* color = strtok(NULL, "_");

        // printf("DEBUG: DEBUG3: file2 == %s\n", file2);
        
        // creates input color
        if(!strcmp(color, "red"))
        {
            red = 255;
            green = 0;
            blue = 0;
        }
        else if(!strcmp(color, "lime"))
        {
            red = 0;
            green = 255;
            blue = 0;
        }
        else if(!strcmp(color, "blue"))
        {
            red = 0;
            green = 0;
            blue = 255;
        }
        else if(!strcmp(color, "yellow"))
        {
            red = 255;
            green = 255;
            blue = 0;
        }
        else if(!strcmp(color, "cyan") || !strcmp(color, "aqua"))
        {
            red = 0;
            green = 255;
            blue = 255;
        }
        else if(!strcmp(color, "magenta") || !strcmp(color, "fuchsia"))
        {
            red = 255;
            green = 0;
            blue = 255;
        }
        else if(!strcmp(color, "maroon"))
        {
            red = 128;
            green = 0;
            blue = 0;
        }
        else if(!strcmp(color, "olive"))
        {
            red = 128;
            green = 128;
            blue = 0;
        }
        else if(!strcmp(color, "green"))
        {
            red = 0;
            green = 128;
            blue = 0;
        }
        else if(!strcmp(color, "purple"))
        {
            red = 128;
            green = 0;
            blue = 128;
        }
        else if(!strcmp(color, "teal"))
        {
            red = 0;
            green = 128;
            blue = 128;
        }
        else if(!strcmp(color, "navy"))
        {
            red = 0;
            green = 0;
            blue = 128;
        }   
        else if(color == NULL)  //if no color was entered
        {
            printf("Error! Color not specified\n\n");
            fclose(fpfile1);
            fclose(fpfile2);
            remove(file2);
            printf("Oops...file named '%s' got corrupted! File deleted automatically\n", file2);
            exit(1);
        }
        else                    //wrong input as color
        {
            printf("Error! Color named '%s' does not exists\n", color);
            fclose(fpfile1);
            fclose(fpfile2);
            remove(file2);
            printf("Oops...file named '%s' got corrupted! File deleted automatically\n", file2);
            exit(1);
        }

        // finds input color percentage
        char* number = strtok(NULL, "\0");
        int percentage = atoi(number);
        if(percentage > 100 || percentage < 0)
        {
            printf("Error with entered percentage!\n");
            fclose(fpfile1);
            fclose(fpfile2);
            remove(file2);
            printf("Oops...file named '%s' got corrupted! File deleted automatically\n", file2);
            exit(1);
        }
        agrey(mainarray, duparray, x, y, red, green, blue, percentage, fpfile1, fpfile2);
    }
    else if(command[0] == '-' && (command[1] == 'p' || command[1] == 'P') && (command[2] > 47 && command[2] < 58))                         
    {
        // printf("DEBUG MESSAGE: INSIDE COMMANDS IF STATEMENT\n");
        int restriction_num;

        // pallet color restriction
        strtok(command,"p");
        restriction_num = atoi(strtok(NULL,"\0"));
        if(restriction_num < 256 && restriction_num % 2 == 0)
        {
            palette(mainarray, duparray, x, y, restriction_num);
        }
        else
        {
            printf("Error: command was wrong. Terminating programm\n");

            // frees everything and exits
            fclose(fpfile1);
            fclose(fpfile2);
            free_all_mem(mainarray, duparray, x, h1, h2, file1, file2);
        }
    }
    /* BONUS COMMANDS */
    else if(commandcheck(command, "-b"))
    {
        // regular grey
        rgrey(mainarray, duparray, x, y);
    }
    else if(commandcheck(command, "-d"))
    {
        // dublicates image
        dublicate(mainarray, duparray, x, y);
    }
    else if(commandcheck(command, "-rl"))
    {
        // rotate image to the left
        // printf("DEBUG FLAAAG\n");
        duparray = realloc(duparray, y * sizeof(rgbe*));
        // printf("DEBUG FLAAA2\n");
        for(i = 0; i < y; i++)
        {
            duparray[i] = realloc(duparray[i], x * sizeof(rgbe));
        }
        // printf("DEBUG FLAAA3\n");

        rotate_left(mainarray, duparray, x, y);

        // switching image dimensions
        h2->bmiHeader.biHeight = y;
        h2->bmiHeader.biWidth  = x;

        // switching array dimensions to match universal fwrite
        int temp;
        temp = x;
        x = y;
        y = temp;
    }
    else if(commandcheck(command, "-rr"))
    {
        // rotate image to the right
        // printf("DEBUG MESSAGE: before rotate_right\n");
        duparray = realloc(duparray, y * sizeof(rgbe*));
        for(i = 0; i < y; i++)
        {
            duparray[i] = realloc(duparray[i], x * sizeof(rgbe));
        }
        rotate_right(mainarray, duparray, x, y);

        // switching image dimensions
        h2->bmiHeader.biHeight = y;
        h2->bmiHeader.biWidth  = x;

        // switching array dimensions to match universal fwrite
        int temp;
        temp = x;
        x = y;
        y = temp;
    }
    else if(commandcheck(command, "-ru"))
    {
        // printf("DEBUG MESSAGE: before rotate_right\n");
        // rotate image 180 degrees
        rotate_up(mainarray, duparray, x, y);
    }
    /* BONUS COMMANDS */
    else
    {
        printf("Invalid input. Command '%s' does not exist\n", argv[1]);
        freearrays(mainarray, duparray, x);
    }
    
    // printf("DEBUG MESSAGE: outputcheck == %d\n", outputcheck);\
    
    // output file check
   
    writeHeader(fpfile2, h2);
    writeInfo(fpfile2, h2);

    /* printf("HEAVY DEBUGGING MESSAGE:\n\n");
    for(i = 0; i < x; i++)
    {
        for(j = 0; j < y; j++)
        {
            // printf("duparray[%d][%d]: blue:%d green%d: red:%d\n",i, j, duparray[i][j].blue, duparray[i][j].green, duparray[i][j].red);
            fwrite(&(duparray[i][j].blue), sizeof(unsigned char), 1, fpfile2);
            fwrite(&(duparray[i][j].green), sizeof(unsigned char), 1, fpfile2);
            fwrite(&(duparray[i][j].red), sizeof(unsigned char), 1, fpfile2);
        }
    } */

    // calculating padding again in case of new dimensions
    extrabytes = 0;
    while((y * 3 + extrabytes) % 4 != 0)
    {
        extrabytes++;
    }

    // write output file from secondary array (duparray) 
    unsigned char nullbyte = 0;
    for(i = (x-1); i >= 0; i--)
    {
        for(j = 0; j < y; j++)
        {
            // printf("DEBUG: good so far i == %d j == %d\n", i, j);
            fwrite(&(duparray[i][j].blue), sizeof(unsigned char), 1, fpfile2);
            fwrite(&(duparray[i][j].green), sizeof(unsigned char), 1, fpfile2);
            fwrite(&(duparray[i][j].red), sizeof(unsigned char), 1, fpfile2);
        }
        for(z = 0; z < extrabytes; z++)
        {
            fwrite(&(nullbyte), sizeof(unsigned char), 1, fpfile2);
        }
        loading_screen ("finalizing image", (x-1)-i, x-1, 3);
    }
    // printf("DEBUG: extrabytes == %d\n", extrabytes);

    // closing used files to avoid corruption
    fclose(fpfile1);
    if(argc > 3)
    {
        fclose(fpfile2);
    }
    printf("\nProcess successful\n");

    //frees all allocated memmory
    if(commandcheck(command, "-ndl") || commandcheck(command, "-ndr"))
    {
        int temp = x;
        x = y;
        y = temp;
        free_all_mem2(mainarray, duparray, x, y, h1, h2, file1, file2);
    }
    else
    {
        free_all_mem(mainarray, duparray, x, h1, h2, file1, file2);
    }
    
    return 0;
}