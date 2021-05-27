#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <pthread.h>

int dim, num_threads;
int **a, **b, **m;
FILE *fPtr;

void *multiplier(void *arg)
{
    int sum;
    int thread_id, interval, start_col, end_col;

    thread_id = *(int *)(arg);
    interval = dim / num_threads;
    start_col = thread_id * interval;
    end_col = (thread_id + 1) * interval;

    for (int i = 0; i < dim; ++i)
    {
        for (int j = start_col; j < end_col; ++j)
        {
            sum = 0;

            for (int k = 0; k < dim; ++k)
            {
                sum += a[i][k] * b[k][j];
            }
            m[i][j] = sum;
        }
    }
}

int main(void)
{
    char filename[32];
    char a_file[32];
    char b_file[32];
    int filename_sizes[6] = {128, 256, 512, 1024, 2048, 4096};
    int thread_sizes[5] = {1, 2, 4, 8, 16};

    struct timeval multiply_start, multiply_stop;
    struct timeval load_start, load_stop;
    struct timeval total_start, total_stop;
    pthread_t *threads;

    for (int q = 0; q < 6; q++)
    {

        for (int ti = 0; ti < 5; ++ti)
        {
            gettimeofday(&total_start, NULL);

            dim = filename_sizes[q];
            num_threads = thread_sizes[ti];
            threads = (pthread_t *)malloc(num_threads * sizeof(pthread_t));

            gettimeofday(&load_start, NULL);

            a = (int **)malloc(sizeof(int *) * dim);
            b = (int **)malloc(sizeof(int *) * dim);
            m = (int **)malloc(sizeof(int *) * dim);

            for (int i = 0; i < dim; i++)
            {
                a[i] = (int *)malloc(sizeof(int) * dim);
                b[i] = (int *)malloc(sizeof(int) * dim);
                m[i] = (int *)malloc(sizeof(int) * dim);
            }

            snprintf(filename, sizeof(char) * 32, "data/resultparallel%i-%i.txt", dim, num_threads);
            snprintf(a_file, sizeof(char) * 32, "data/a%i.txt", dim);
            snprintf(b_file, sizeof(char) * 32, "data/b%i.txt", dim);

            // read-a
            FILE *pf;
            pf = fopen(a_file, "r");

            for (size_t i = 0; i < dim; ++i)
            {
                for (size_t j = 0; j < dim; ++j)
                {
                    fscanf(pf, "%d", a[i] + j);
                }
            }

            // read-b
            pf = fopen(b_file, "r");
            for (size_t i = 0; i < dim; ++i)
            {
                for (size_t j = 0; j < dim; ++j)
                {
                    fscanf(pf, "%d", b[i] + j);
                }
            }

            gettimeofday(&load_stop, NULL);

            fPtr = fopen(filename, "w");

            if (fPtr == NULL)
            {
                printf("Unable to create file.\n");
                exit(EXIT_FAILURE);
            }

            gettimeofday(&multiply_start, NULL);

            for (int i = 0; i < num_threads; ++i)
            {
                int *thread_id;
                thread_id = (int *)malloc(sizeof(int));
                *thread_id = i;
                pthread_create(&threads[i], NULL, multiplier, (void *)thread_id);
            }

            for (int i = 0; i < num_threads; ++i)
            {
                pthread_join(threads[i], NULL);
            }

            gettimeofday(&multiply_stop, NULL);
            gettimeofday(&total_stop, NULL);

            for (int i = 0; i < dim; ++i)
            {
                for (int j = 0; j < dim; ++j)
                {
                    fprintf(fPtr, "%-5d ", m[i][j]);
                }
                fprintf(fPtr, "\n");
            }

            fprintf(fPtr, "\nMultiplication time: %3.5f seconds\n", ((multiply_stop.tv_sec - multiply_start.tv_sec) * 1000000 + multiply_stop.tv_usec - multiply_start.tv_usec) / 1000000.0);
            fprintf(fPtr, "\nLoading files time: %3.5f seconds\n", ((load_stop.tv_sec - load_start.tv_sec) * 1000000 + load_stop.tv_usec - load_start.tv_usec) / 1000000.0);
            fprintf(fPtr, "\nTotal time: %3.5f seconds\n", ((total_stop.tv_sec - total_start.tv_sec) * 1000000 + total_stop.tv_usec - total_start.tv_usec) / 1000000.0);

            printf("%dx%d multiplication with %d thread \t\t--> done\n", dim, dim, num_threads);
        }
        printf("\n");
    }
    printf("\nAll multiplications done.\n");

    return 0;
}
