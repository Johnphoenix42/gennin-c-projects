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

double calcPearsonsCorrelationCoefficient(double *xValues, double *yValues, size_t n)
{
    double sumX, sumY, sumXSquared, sumYSquared, sumXY;
    double ssxy, ssxx, ssyy;
    int i;

    for (i = 0; i < n; i++) {
        sumX += *xValues;
        sumY += *yValues;
        sumXSquared += pow(*xValues, 2);
        sumYSquared += pow(*yValues, 2);
        sumXY += *xValues * *yValues;
        xValues++;
        yValues++;
    }
    ssxy = calcSSxy(n, sumXY, sumX, sumY);
    ssxx = calcSSxx(n, sumXSquared, sumX);
    ssyy = calcSSyy(n, sumYSquared, sumY);
    return ssxy/sqrt(ssxx * ssyy);
}

int main (int argc, char **argv) {
    Pair *pairArr;
    Pair pair;
    size_t n = 0;
    int scanRes;
    /*for (int i = 0; i < argc; i++) {
        printarg(argv[i]);
    }*/
    pairArr = calloc(0, sizeof(Pair));
    printf("Enter sets of point in the format x,y\n");
    scanRes = scanf("%lf,%lf", &pair.x, &pair.y);
    while (scanRes == 2) {
        Pair *pairPtr = realloc(pairArr, (n + 1) * sizeof(Pair));
        if (pairPtr == NULL) printf("Pair struct was not succesfully allocated\n");
        for (int i = 0; i < n; i++){
            *(pairPtr + i) = *(pairArr + i);
        }
        (pairPtr + n)->x = pair.x;
        (pairPtr + n)->y = pair.y;
        pairArr = pairPtr;
        printf("pointer assigned\n");
        n++;
        printf("Got to the end of the loop\n");
        scanRes = scanf("%lf,%lf", &pair.x, &pair.y);
        printf("Loop started;  scanRes = %d\n", scanRes);
    }
    free(pairArr);
    return EXIT_SUCCESS;
}
