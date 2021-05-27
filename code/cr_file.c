#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

int main()
{
    int check;
    char *dirname = "data";

    #ifdef __linux__
        check = mkdir(dirname,0700);
    #else
        check = mkdir(dirname);
    #endif

    // check if directory is created or not
    if (!check){
        printf("Directory created\n");
    }


    char filename[32];

    char filenames[2][2] = {"a", "b"};
    int filename_sizes[6] = {128, 256, 512, 1024, 2048, 4096};

    FILE *fPtr;

    int dim = 10;

    for (int w = 0; w < 2; w++)
    {
        for (int k = 0; k < 6; k++)
        {
            snprintf(filename, sizeof(char) * 32, "data/%s%i.txt", filenames[w], filename_sizes[k]);
            fPtr = fopen(filename, "w");

            if (fPtr == NULL)
            {
                printf("Unable to create file.\n");
                exit(EXIT_FAILURE);
            }

            dim = filename_sizes[k];

            for (int i = 0; i < dim; i++)
            {
                for (int j = 0; j < dim; j++)
                {
                    fprintf(fPtr, "%d ", rand() % 10);
                }
                fprintf(fPtr, "\n");
            }
            printf("%s%i \t--> created\n", filenames[w], filename_sizes[k]);
            fclose(fPtr);
        }
    }

    printf("\nFiles created and saved successfully.\n");
    return 0;
}
