#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>

#define NO_OF_STUDENTS 10

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
    return 0;
}
