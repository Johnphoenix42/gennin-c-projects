#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define NO_OF_STUDENTS 10

int comp (const void *a, const void *b) 
{
    int arg1 = *(const int *)a;
    int arg2 = *(const int *)b;
    if (arg1 > arg2) return 1;
    else if (arg1 < arg2) return -1;
    else return 0;
}

int main()
{
    int studentScores[NO_OF_STUDENTS];
    srand(time(NULL));
    printf("%d\n", sizeof(RAND_MAX));
    int *studentCounter = studentScores;
    /*while (studentCounter - studentScores < NO_OF_STUDENTS) {
        int score = rand() % 101;
        *studentCounter = score;
        ++studentCounter;
        printf(">>%p\n", studentCounter);
    }*/
    for (int *studentCounter = studentScores; studentCounter < studentScores + NO_OF_STUDENTS; ++studentCounter) {
        int score = rand() % 101;
        *studentCounter = score;
        printf(">>%p\n", studentCounter);
    }
    printf("%d, %d, %d\n", *studentScores, studentScores[1], studentScores[2]);

    size_t size = 1000000;
    float *val = malloc(size * sizeof(float));
    for (int i = 0; i < size; i++) {
        val[i] = rand() % 101;
    }
    clock_t start = clock();
    qsort(val, size, sizeof(float), comp);
    clock_t end = clock();
    double time_taken = (double)(end - start) / CLOCKS_PER_SEC;
    printf("Sorting time = %lf\n", time_taken);
    return 0;
}
