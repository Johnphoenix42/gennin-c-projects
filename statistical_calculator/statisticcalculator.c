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
    int i;

    for (i = 0; i < n; i++) {
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

int main (int argc, char **argv) {
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
