#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#define EXIT_SUCCESS 0

typedef struct node {
    int value;
    struct node *next;
} Node, *NodePtr;

typedef struct pair {
    double x, y;
} Pair, *PairPtr;

typedef struct throuple {
    int index; 
    double data, rank;
} Throuple;

void printarg(char *arg) {
    printf("arg: %s\t%zu\t(", arg, strlen(arg));
    for (int i = 0; i < strlen(arg); i++) {
        printf(" %02x", arg[i]);
    }
    printf(" )\n");
}

double calcSSxy(size_t n, double sumXY, double sumX, double sumY)
{
    return sumXY - sumX * sumY / n;
}

double calcSSxx(size_t n, double sumXX, double sumX)
{
    return sumXX - sumX * sumX / n;
}

double calcSSyy(size_t n, double sumYY, double sumY)
{
    return sumYY - sumY * sumY / n;
}

double calcPearsonsCorrelationCoefficient(Pair *xyPair, size_t n)
{
    double sumX, sumY, sumXSquared, sumYSquared, sumXY;
    double ssxy, ssxx, ssyy;

    for (int i = 0; i < n; i++) {
        sumX += (xyPair + i)->x;
        sumY += (xyPair + i)->y;
        sumXSquared += pow((xyPair + i)->x, 2);
        sumYSquared += pow((xyPair + i)->y, 2);
        sumXY += (xyPair + i)->x * (xyPair + i)->y;
    }
    ssxy = calcSSxy(n, sumXY, sumX, sumY);
    ssxx = calcSSxx(n, sumXSquared, sumX);
    ssyy = calcSSyy(n, sumYSquared, sumY);
    return ssxy/sqrt(ssxx * ssyy);
}

Pair *get_rank(const Pair *xyPair, size_t n)
{
    //Generate a throuple for each of the xyPair column
    Throuple mthrouple[2][n], sortedThrouple[2][n];
    for (int j = 0; j < 2; ++j) {
        for (int i = 0; i < n; ++i) {
            mthrouple[j][i].index = i;
            mthrouple[j][i].data = (xyPair + i)->x;
            sortedThrouple[j][i].index = i;
            sortedThrouple[j][i].data = (xyPair + i)->y;
        }
    }
    // sort through sortedThrouple
    for (int h = 0; h < 2; ++h) {
        for (int i = 0; i < n; ++i) {
            double temp_data = sortedThrouple[h][i].data;
            for (int j = i - 1; j >= 0; --j) {
                if (sortedThrouple[h][j].data < temp_data) {
                    sortedThrouple[h][j].data = temp_data;
                    break;
                }
                sortedThrouple[h][j+1].data = sortedThrouple[h][j].data;
            }
        }
    }
}

double calculateSpearmanRankCoefficient(const Pair *xyPair, size_t n) 
{
    // x_rank.x is the value, x_rank.y is the rank;
    Pair *rank = get_rank(xyPair, n);
    double rank_diff_squared = 0;
    for (int i = 0; i < n; i++) {
        rank_diff_squared += pow(rank->y - (rank+1)->y, 2);
    }
    return 1 - (6 * rank_diff_squared) / (pow(n, 3) - n);
}

int main (int argc, char **argv) {
    //What is the difference between char * and const char * in formal parameters

    Pair *pairPtr = NULL;
    Pair pair;
    size_t n = 0;
    int scanRes;
    /*for (int i = 0; i < argc; i++) {
        printarg(argv[i]);
    }*/
    printf("Enter sets of point in the format x,y\tBreak the rule to exit\n");
    while ((scanRes = scanf("%lf,%lf", &pair.x, &pair.y)) == 2) {
        pairPtr = realloc(pairPtr, (n + 1) * sizeof(Pair));
        if (pairPtr == NULL) printf("Pair struct was not succesfully allocated\n");
        (pairPtr + n)->x = pair.x;
        (pairPtr + n)->y = pair.y;
        //printf("pointer assigned\n");
        n++;
    }
    for (size_t i = 0; i < n; i++) {
        printf("%.4lf\t%.4lf\n", (pairPtr + i)->x, (pairPtr + i)->y);
    }
    double pearsonCorrelationCoefficient = calcPearsonsCorrelationCoefficient(pairPtr, n);
    printf("pearson correlation coefficient = %.4lf\n", pearsonCorrelationCoefficient);
    free(pairPtr);
    return EXIT_SUCCESS;
}

void *sort_numbers(int *array, size_t n) {
    for (int i = 1; i < n; i++) {
        int temp = *(array + i);
        for (int j = i - 1; j >= 0; j--) {
            if (temp > *(array + j)){
                *(array + j + 1) = *(array + j);
            }
            *(array + j) = temp;
        }
        printf("%d\n", i);
    }
}
