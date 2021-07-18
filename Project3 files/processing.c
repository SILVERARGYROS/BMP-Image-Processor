/* Argyros Konstantinos
AM: 2022202000014
dit20014@go.uop.gr

Pavlos Sygrimis
AM: 2022202000202
dit20202@go.uop.gr */

//foo.c

#include "bmp.h"
#include "processing.h"

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
    loading_screen ("dublicating image", i, x, 5);
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
        printf("DEBUG MESSAGE: choto matte wtf\n");
        loading_screen ("flipping image", i, x-1, 5);
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
        loading_screen ("removing colors", i, x-1, 5);
    }
    printf("DEBUG MESSAGE: left rgrey\n");
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
        }
        loading_screen ("rotating image", i, x-1, 5);
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
        }
        loading_screen ("rotating image", i, x-1, 5);
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
        }
        loading_screen ("rotating image", i, x-1, 5);
    }
}



void loading_screen (char message[], int base, int top, int miliseconds)
{
    float completion_percentage;

    printf("\r%s",message);
    completion_percentage =  ((float) (base)/top)*100;
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
    delay(miliseconds); // for smooth cosmetic touch only. Heavy commands need 0 delay
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

void palette (rgbe** mainarray, rgbe** duparray, int x, int y, int restriction_num)
{
    printf("DEBUG MESSAGE: inside pallete\n");

    int i,j;
    int space, base, top;

    // pallet restriction
    space = 256 / restriction_num;

    // pixel conversion

    for(i = 0; i < x; i++)
    {
        for(j = 0; j < y; j++)
        {
            for(base = 0; base < 256; base = base + space)
            {
                /* if(!i && !j)
                {
                    printf("DEBUG: base == %d\n", base);
                    printf("DEBUG: top == %d\n", base + space - 1);
                } */
                top = base + space - 1;
                if(mainarray[i][j].red >= base && mainarray[i][j].red <= top)
                {
                    duparray[i][j].red = (unsigned char) base;
                    break;
                }
            }
            for(base = 0; base < 256; base = base + space)
            {
                top = base + space - 1;
                if(mainarray[i][j].green >= base && mainarray[i][j].green <= top)
                {
                    duparray[i][j].green = (unsigned char) base;
                    break;
                }
            }

            for(base = 0; base < 256; base = base + space)
            {
                top = base + space - 1;
                if(mainarray[i][j].blue >= base && mainarray[i][j].blue <= top)
                {
                    duparray[i][j].blue = (unsigned char) base;
                    break;
                }
            }

        }
        loading_screen ("changing colors", i, x-1, 5);
    }
    printf("DEBUG MESSAGE: leaving pallete\n");
}

void agrey (rgbe** mainarray, rgbe** duparray, int x, int y, int red, int green, int blue, int percentage, FILE* fpfile1, FILE* fpfile2)
{
    int i, j, quantity, count;
    float color_distance;
    unsigned char pred, pgreen, pblue;
    float temp1;
    int temp2, temp3;
    int numofpixels, k, l;
    int n;

    printf("DEBUG: inside agrey\n");


    rgrey(mainarray, duparray, x, y);
    quantity = 0;
    count = 0;

    // allocating memory for helping arrays
    float* distance_array;
    distance_array = (float*) malloc (sizeof(float) *x*y);
    if(distance_array == NULL)
    {
        fclose(fpfile1);
        fclose(fpfile2);
        printf("Memory error! Please download more ram next time\n");
        exit(0);
    }

    int** pixelcords_array;
    pixelcords_array = (int**) malloc (sizeof(int*) *x*y);
    if(pixelcords_array == NULL)
    {
        fclose(fpfile1);
        fclose(fpfile2);
        printf("Memory error! Please download more ram next time\n");
        exit(0);
    }
    for(j = 0; j < x*y; j++)
    {
        pixelcords_array[j] = (int*) malloc (sizeof(int) *2);
        if(pixelcords_array[j] == NULL)
        {
            fclose(fpfile1);
            fclose(fpfile2);
            printf("Memory error! Please download more ram next time\n");
            exit(0);
        }
    }

    printf("DEBUG MESASAGE: after all mallocs\n");
    int numofele;
    numofele = 0;
    int inputhue;
    // filling helping arrays
    for(i = 0; i < x; i++)
    {
        for(j = 0; j < y; j++)
        {
            pred    = mainarray[i][j].red;
            pgreen  = mainarray[i][j].green;
            pblue   = mainarray[i][j].blue;

            inputhue = calculating_hue(red, green, blue);
            distance_array[(i*y)+j] = euclidean_distance(red, green, blue, pred, pgreen, pblue) + abs(inputhue - calculating_hue(pred, pgreen, pblue));

            pixelcords_array[(i*y)+j][0] = i;
            pixelcords_array[(i*y)+j][1] = j;
            // printf("Inside sort\n");
            sort_element(distance_array, pixelcords_array, numofele);
            numofele++;
            // loading_screen ("setting up variables", i*y + j, (x-1) * (y-1), 0);
        }
        loading_screen ("setting up variables", i, (x-1), 0);
        // printf("===================\n");
    }

    printf("going in second\n");
    
    // sorting the arrays (bubblesort)
    /* for(i = 1; i < x*y; i++)
    {
        for(j = x*y-1; j >= i; j--)
        {
            if(distance_array[j-1] > distance_array[j])
            {
                // sorting distance_array
                temp1 = distance_array[j-1];
                distance_array[j-1] = distance_array[j];
                distance_array[j] = temp1;
                
                // sorting pixelcords_array[0]
                temp2 = pixelcords_array[0][j-1];
                pixelcords_array[0][j-1] = pixelcords_array[0][j];
                pixelcords_array[0][j] = temp2;

                // sorting pixelcords_array[1]
                temp2 = pixelcords_array[1][j-1];
                pixelcords_array[1][j-1] = pixelcords_array[1][j];
                pixelcords_array[1][j] = temp2;
            }
        }
        loading_screen ("adding final touches", i, y*x-1, 0);
    } */


    // sorting the arrays (quicksort)
    /* n = x*y;
    printf("DEBUG MESSAGE: n == %d\n", n);
    quickSort(distance_array, 0, n - 1, pixelcords_array); */

    
    // debugging block
    /* for(i = 0; i < x*y; i++)
    {
        printf("distance[%d] == %f  ", i, distance_array[i]);
        printf("distance[%d] == %d  ", i, pixelcords_array[0][i]);
        printf("distance[%d] == %d\n", i, pixelcords_array[1][i]);
    } */

    // exit(0);
    printf("DEBUG: After exit\n");

    numofpixels = (x*y) * percentage / 100;
    printf("DEBUG: After numofpixels declaration\n");

    for(i = 0; i < numofpixels; i++)
    {

        k = pixelcords_array[i][0];
        l = pixelcords_array[i][1];

        printf("DEBUG: inside for k == %d l == %d\n", k, l);

        duparray[k][l].red = mainarray[k][l].red;      
        duparray[k][l].green = mainarray[k][l].green;      
        duparray[k][l].blue = mainarray[k][l].blue;      
    }
    printf("DEBUG: After array filling\n");

    free(distance_array);
    printf("DEBUG: After freeing distance_array\n");

    for(i = 0; i < x*y; i++)
    {
        free(pixelcords_array[i]);
    }

    free(pixelcords_array);
    printf("DEBUG: After after free\n");

    printf("DEBUG: outside agrey\n");
}

void sort_element (float* distance_array, int** pixelcords_array, int numofele)
{
    int temp2;
    float temp1;
    while(numofele > 0)
    {
        if(distance_array[numofele - 1] > distance_array[numofele])
        {
            // sorting distance_array
            temp1 = distance_array[numofele-1];
            distance_array[numofele-1] = distance_array[numofele];
            distance_array[numofele] = temp1;

            // printf("after if 1\n");
                            
            // sorting pixelcords_array[0]
            temp2 = pixelcords_array[numofele-1][0];
            pixelcords_array[numofele-1][0] = pixelcords_array[numofele][0];
            pixelcords_array[numofele][0] = temp2;

            // printf("after if 2\n");
                
            // sorting pixelcords_array[1]
            temp2 = pixelcords_array[numofele-1][1];
            pixelcords_array[numofele-1][1] = pixelcords_array[numofele][1];
            pixelcords_array[numofele][1] = temp2;
            // printf("after if 3\n");
            
            numofele--;
        }
        else
        {
            break;
        }
    }
    // printf("outside sort\n");

}

float euclidean_distance (int ired, int igreen, int iblue, int pred, int pgreen, int pblue)
{
    int x1, x2, x3;
    float sum;
    x1 = ired - pred;
    x1 = x1 * x1;

    x2 = igreen - pgreen;
    x2 = x2 * x2;

    x3 = iblue - pblue;
    x3 = x3 * x3;

    sum = x1 + x2 + x3;
    // printf("DEBUG MESSAGE: sum == %.2f\n", sum);
    // sum = sqrt(sum);
    // printf("BEFORE ROOT\n");

    // math.h algorithm
    return sqrt(sum);


    // sum = sqrt(sum);
    // return sum/sqrt((255*255 + 255*255 + 255*255));
    

    // return pow(sum, 2);

    // return sum;

    // debugging tools 

    // printf("DEBUG MESSAGE: sqrt == %f\n", sqrt(sum));
    // exit(0); // Bebug exit

    // printf("after ROOT\n");
    // printf("DEBUG MESSAGE: root == %.2f\n", root);
    // return root;
    return 0;
}

void quickSort(float* arr, int low, int high, int** pixelcords_array)
{
    if (low <= high)
    {
        /* pi is partitioning index, arr[p] is now
        at right place */
        int pi = partition(arr, low, high, pixelcords_array);

        // Separately sort elements before
        // partition and after partition
        quickSort(arr, low, pi - 1, pixelcords_array);
        quickSort(arr, pi + 1, high, pixelcords_array);
    }
}

int partition (float* arr, int low, int high, int** pixelcords_array)
{
    int pivot = arr[high]; // pivot
    int i = (low - 1); // Index of smaller element and indicates the right position of pivot found so far
    int temp;
    float temp2;

    for (int j = low; j <= high - 1; j++)
    {
        // If current element is smaller than the pivot
        if (arr[j] < pivot)
        {
            i++; // increment index of smaller element

            // swap(&arr[i], &arr[j]);
            temp2 = arr[i];
            arr[i] = arr[j];
            arr[j] = temp2;

            temp = pixelcords_array[i][0];
            pixelcords_array[i][0] = pixelcords_array[j][0];
            pixelcords_array[j][0] = temp;

            temp = pixelcords_array[1][i];
            pixelcords_array[i][1] = pixelcords_array[j][1];
            pixelcords_array[j][1] = temp;
        }
    }
    // swap(&arr[i + 1], &arr[high]);
    temp2 = arr[high];
    arr[high] = arr[i + 1];
    arr[i + 1] = temp2;

    temp = pixelcords_array[high][0];
    pixelcords_array[high][0] = pixelcords_array[i + 1][0];
    pixelcords_array[i + 1][0] = temp;

    temp = pixelcords_array[high][1];
    pixelcords_array[high][1] = pixelcords_array[i + 1][1];
    pixelcords_array[i + 1][1] = temp;

    return (i + 1);
}

void free_all_mem (rgbe** mainarray, rgbe** duparray, int x, struct bmp_header *h1, struct bmp_info *h2, char* file1, char* file2)
{
    //frees all allocated memmory
    printf("\nDEBUG MESSAGE: Freeing Process started\n");
    freearrays(mainarray, duparray, x);
    free(h1);
    free(h2);
    free(file1);
    free(file2);
}

void free_all_mem2 (rgbe** mainarray, rgbe** duparray, int x, int y, struct bmp_header *h1, struct bmp_info *h2, char* file1, char* file2)
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
    free(file1);
    free(file2);   
}

int calculating_hue (int red, int green, int blue)
{
    float hue;
    int identifier;
    float min, max;

    /* red = red / 255;
    green = green / 255;
    blue = blue / 255; */


    if(red < green && red < blue)
    {
        min = red;
    }
    else if(green < red && green < blue)
    {
        min = green;
    }
    else if(blue < red && blue < green)
    {
        min = blue;
    }

    if(red >= green && red >= blue)
    {
        // printf("debug inside hue if apo ta red\n");
        max = red;
        identifier = 1;  
    }
    else if(green >= red && green >= blue)
    {
        // printf("debug inside hue if apo ta green\n");

        max = green;
        identifier = 2;
    }
    else if(blue >= red && blue >= green)
    {
        // printf("debug inside hue if apo ta blue\n");

        max = blue;
        identifier = 3;
    }

    if(max == 0)
    {
        // printf("debug inside hue if max\n");
        
        // printf("returns 0\n");
        return 0;
    }

    if(identifier == 1)
    {
        // printf("debug inside hue if id 1\n");

        hue = (green - blue)/(max-min);
    }
    else if(identifier == 2)
    {
        // printf("debug inside hue if id 2\n");

        hue = 2 + (blue - red)/(max-min);
    }
    else if(identifier == 3)
    {
        // printf("debug inside hue if id 3\n");

        hue = 4 + (red - green)/(max-min);
    }
        
        

    hue = hue * 60;

    if(hue < 0)
    {
        hue = hue + 360;
    }
    // printf("returns hue == %f\n", hue);
    return (int) hue;
}