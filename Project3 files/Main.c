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
    char* file1;
    char* file2;
    // searching and opening files 
    
    file1 = strdup(argv[2]);
    strcat(file1, ".bmp");  // rephrasing real file name (adding .bmp)


    // debugging block
    printf("DEBUG: argv[2] == %s\n", argv[2]);
    printf("DEBUG: file1 == %s\n", file1);
    printf("=======\n");


    fpfile1 = fopen(file1, "r");
    if(fpfile1 == NULL)
    {
        printf("file named '%s' could not be opened\n", file1);
        exit(1);
    }
    printf("DEBUG MESSAGE: argc == %d\n", argc);
    if(argc > 3)
    {   
        // printf("DEBUG MESSAGE: before if statement\n");
        file2 = strdup(argv[3]);
        // printf("DEBUG MESSAGE: after if statement\n");
        strcat(file2, ".bmp");  // rephrasing real file name (adding .bmp)

        if(!strcmp(file1, file2))
        {
            printf("Input file and output file must NOT be the same file.\n");
            fclose(fpfile1);
            exit(1);
        }
        if(fpfile2 = fopen(file2, "r"))
        {
            int answer;
            printf("file named '%s' already exists. Are you sure you want to overwrite?? (0 == yes || 1 == no)\n", file2);
            scanf("%d", &answer);
            if(answer)
            {
                fclose(fpfile1);
                fclose(fpfile2);
                printf("Exiting Programm\n");
                exit(0);
            }
        }
        else
        {
            fclose(fpfile2);
        }
        // if command != "-a" then we have putput file
        printf("DEBUG: argv[3] == %s\n", argv[3]);
        printf("DEBUG: DEBUG: file2 == %s\n", file2);
        fpfile2 = fopen(file2, "w");
        if(fpfile2 == NULL)
        {
            printf("file named '%s' could not be opened\n", file2);
            fclose(fpfile1);         //closing already opened files
            exit(1);
        }
    }

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


    //allocating memory for main array
    mainarray = (rgbe**) malloc(sizeof(rgbe*) * x);
    if(mainarray == NULL)
    {
        printf("Not enough memory. Please download more ram 3.\n");
        fclose(fpfile1);
        fclose(fpfile2);
        exit(1);
    }
    for(i = 0; i < x; i++)
    {
        mainarray[i] = (rgbe*) malloc(sizeof(rgbe) * y);
        if(mainarray[i] == NULL)
        {
            printf("Not enough memory. Please download more ram 4.\n");
            fclose(fpfile1);
            fclose(fpfile2);
            exit(1);
        }
    }
    // dont forget the Putin
    int extrabytes = 0;
    while((y * 3 + extrabytes) % 4 != 0)
    {
        extrabytes++;
    }
    // printf("DEBUG: extrabytes == %d\n", extrabytes);

    // printf("DEBUG x == %d", x);
    printf("DEBUG before animation\n");
    printf("\n");

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
            // fread(&mainarray[i][j], sizeof(rgbe), 1, fpfile1);
        }
        loading_screen ("loading image",(x-1)-i, x-1);
        fseek(fpfile1, extrabytes, SEEK_CUR); //skips bytes from patting
    }
    printf("DEBUG: good so far (read 1st)\n");

        printf("DEBUG: DEBUG1.5.5.5.5.5: file2 == %s\n", file2);

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
    int outputcheck = 1;        // flag to produce output file

    if(commandcheck(command, "-a"))                              
    {
        // prints attributes
        printattributes(h1, h2);
        outputcheck = 0;         // doesnt have output file name
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
        printf("DEBUG: DEBUG3: file2 == %s\n", file2);

        
        char* color = strtok(NULL, "_");

        if(!strcmp(color, "red"))
        {
            red = 255;
            green = 0;
            blue = 0;
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
        else if(!strcmp(color, "yellow"))
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
        else if(color == NULL)
        {
            printf("Error! Color not specified\n\n");
            fclose(fpfile1);
            fclose(fpfile2);
            remove(file2);
            printf("Oops...file named '%s' got corrupted! File deleted automatically\n", file2);
            exit(1);
        }
        else
        {
            printf("Error! Color named '%s' does not exists\n", color);
            fclose(fpfile1);
            fclose(fpfile2);
            remove(file2);
            printf("Oops...file named '%s' got corrupted! File deleted automatically\n", file2);
            exit(1);
        }

        char* number = strtok(NULL, "_");
        int percentage = atoi(number);
        if(percentage > 100 || percentage < 0)
        {
            printf("ERROR!\n");
            fclose(fpfile1);
            fclose(fpfile2);
            remove(file2);
            printf("Oops...file named '%s' got corrupted! File deleted automatically\n", file2);
            exit(1);
        }
        agrey(mainarray, duparray, x, y, red, green, blue, percentage);
    }
    else if(command[0] == '-' && (command[1] == 'p' || command[1] == 'P') && (command[2] > 47 && command[2] < 58))                         
    {
        int restriction_num;
        // pallet color restriction
        strtok(command,"p");
        restriction_num = atoi(strtok(NULL,"\0"));
        if(/* restriction_num > 0 &&  */restriction_num < 256 && restriction_num % 2 == 0)
        {
            palette(mainarray, duparray, x, y, restriction_num);
        }
        else
        {
            printf("DEBUG MESSAGE: command was wrong. Please try again\n");
            // frees everything

            exit(1);
        }
    }
    /* BONUS COMMANDS*/
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
    else if(commandcheck(command, "-ndl"))
    {
        // rotate image -90 degrees
        // printf("DEBUG FLAAAG\n");
        duparray = realloc(duparray, y * sizeof(rgbe*));
        // printf("DEBUG FLAAA2\n");

        for(i = 0; i < y; i++)
        {
            duparray[i] = realloc(duparray[i], x * sizeof(rgbe));
        }
        // printf("DEBUG FLAAA3\n");

        rotate_left(mainarray, duparray, x, y);
        h2->bmiHeader.biHeight = y;
        h2->bmiHeader.biWidth  = x;
        int temp = x;
        x = y;
        y = temp;
    }
    else if(commandcheck(command, "-ndr"))
    {
        // rotate image 90 degrees
        printf("DEBUG MESSAGE: before rotate_left\n");
        printf("DEBUG MESSAGE: before rotate_left\n");
        duparray = realloc(duparray, y * sizeof(rgbe*));
        for(i = 0; i < y; i++)
        {
            duparray[i] = realloc(duparray[i], x * sizeof(rgbe));
        }
        rotate_right(mainarray, duparray, x, y);
        h2->bmiHeader.biHeight = y;
        h2->bmiHeader.biWidth  = x;
        int temp;
        temp = x;
        x = y;
        y = temp;
    }
    else if(commandcheck(command, "-oed"))
    {
        printf("DEBUG MESSAGE: before rotate_right\n");
        // rotate image 180 degrees
        rotate_up(mainarray, duparray, x, y);
    }
    /* BONUS COMMANDS*/
    else
    {
        printf("Invalid input. Command '%s' does not exist\n", argv[1]);
        freearrays(mainarray, duparray, x);
        /* free(mainarray);
        free(duparray); */
        exit(1);
    }
    
    printf("DEBUG MESSAGE: outputcheck == %d\n", outputcheck);
    if(outputcheck)
    {
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
            loading_screen ("finalizing image", (x-1)-i, x-1);
        }
        // printf("DEBUG: extrabytes == %d\n", extrabytes);
    }

    // closing used files
    fclose(fpfile1);
    if(argc > 3)
    {
        fclose(fpfile2);
    }

    //frees all allocated memmory
    if(commandcheck(command, "-ndl") || commandcheck(command, "-ndr") || commandcheck(command, "-oed"))
    {
        int temp = x;
        x = y;
        y = temp;
        free_all_mem2(mainarray, duparray, x, y, h1, h2);
    }
    else
    {
        free_all_mem(mainarray, duparray, x, h1, h2);
    }


    /* freearrays(mainarray, duparray, x);
    free(h1);
    free(h2); */

    printf("\nProcess successful\n");
    // printf("DEBUG MESSAGE: programm exiting normally\n");
    return 0;
}