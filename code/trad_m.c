#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <sys/time.h>

int main(void)
{
    int sum;
    char filename[32];
    char a_file[32];
    char b_file[32];
    int filename_sizes[6] = {128, 256, 512, 1024, 2048, 4096};
    struct timeval start, stop;

    FILE *fPtr;

    for (int q = 0; q < 6; q++)
    {
        int dim = filename_sizes[q];

        int **a;
        int **b;
        int **m;

        a = (int **)malloc(sizeof(int *) * dim);
        b = (int **)malloc(sizeof(int *) * dim);
        m = (int **)malloc(sizeof(int *) * dim);

        for (int i = 0; i < dim; i++)
        {
            a[i] = (int *)malloc(sizeof(int) * dim);
            b[i] = (int *)malloc(sizeof(int) * dim);
            m[i] = (int *)malloc(sizeof(int) * dim);
        }

        snprintf(filename, sizeof(char) * 32, "data/result%i.txt", dim);
        snprintf(a_file, sizeof(char) * 32, "data/a%i.txt", dim);
        snprintf(b_file, sizeof(char) * 32, "data/b%i.txt", dim);

        // read a-file
        FILE *pf;
        pf = fopen(a_file, "r");

        for (size_t i = 0; i < dim; ++i)
        {
            for (size_t j = 0; j < dim; ++j)
            {
                fscanf(pf, "%d", a[i] + j);
            }
        }

        // read b-file
        pf = fopen(b_file, "r");

        for (size_t i = 0; i < dim; ++i)
        {
            for (size_t j = 0; j < dim; ++j)
            {
                fscanf(pf, "%d", b[i] + j);
            }
        }

        fPtr = fopen(filename, "w");

        if (fPtr == NULL)
        {
            printf("Unable to create file.\n");
            exit(EXIT_FAILURE);
        }

        // multiply
        gettimeofday(&start, NULL);
        for (int i = 0; i < dim; i++)
        {
            for (int j = 0; j < dim; j++)
            {
                sum = 0;
                m[i][j] = 0;
                for (int k = 0; k < dim; k++)
                {
                    sum += a[i][k] * b[k][j];
                }
                m[i][j] = sum;
                // add to file
                fprintf(fPtr, "%-5d ", m[i][j]);
            }
            fprintf(fPtr, "\n");
        }
        gettimeofday(&stop, NULL);
        fprintf(fPtr, "\nMultiplication time: %3.5f seconds\n", ((stop.tv_sec - start.tv_sec) * 1000000 + stop.tv_usec - start.tv_usec) / 1000000.0);
        printf("%dx%d multiplication \t\t--> done\n", dim, dim);
        fclose(fPtr);
    }
    printf("\nAll multiplications done.\n");
    return 0;
}
