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
} Pair;

typedef struct rank_and_pair {
    Pair *ordered_pair;
    Pair *unordered_pair;
    int size;
} RankPair;

RankPair *get_rank(float *data, size_t n);

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

double calculateSpearmanRankCoefficient(float *xData, float *yData, size_t n) 
{
    RankPair *xrank = get_rank(xData, n);
    RankPair *yrank = get_rank(yData, n);
    double rank_diff_squared = 0;
    printf("%-10s%-10s%-10s%-10s%-10s%-10s\n", "XData", "YData", "XRank", "YRank", "d", "d(sqr)");
    for (int i = 0; i < n; ++i) {
        double diff = (xrank -> unordered_pair + i) -> y - (yrank -> unordered_pair + i) -> y;
        rank_diff_squared += pow(diff, 2.0);
        printf("%-10.2f%-10.2f%-10.2f%-10.2f%-10.2f%-10.2f\n", 
            ((xrank->unordered_pair) + i) -> x, ((yrank->unordered_pair) + i) -> x,
            ((xrank->unordered_pair) + i) -> y, ((yrank->unordered_pair) + i) -> y,
            diff, pow(diff, 2.0)
        );
    }
    printf("rank_diff_squared = %.2lf\n", rank_diff_squared);
    return 1 - (6 * rank_diff_squared) / (pow(n, 3) - n);
}

int main (int argc, char **argv) {
    Pair *pairPtr = NULL;
    float *xData = NULL;
    float *yData = NULL;
    Pair pair;
    size_t n = 0;
    int scanRes;

    for (int i = 0; i < argc; i++) {
        printarg(argv[i]);
    }

    printf("Use test values: (Y)/N: ");
    int c = getchar();
    if (c == 'Y' || c == 'y'){
        float xval[10] = {75, 80, 93, 65, 87, 71, 98, 68, 84, 77};
        float yval[10] = {82, 78, 86, 72, 91, 80, 95, 72, 89, 74};
        float data[] = {94, 86, 74, 21, 86, 75, 86};
        //pairPtr = calloc(10, sizeof(Pair));
        xData = xval;
        yData = yval;
        size_t length = sizeof (xval) / sizeof (xval[0]);
        RankPair *xrankPair = get_rank(xData, length);
        RankPair *yrankPair = get_rank(yData, length);
        
        double spearmanCorrelationCoefficient = calculateSpearmanRankCoefficient(xData, yData, length);
        printf("\nspearman rank correlation coefficient = %.4lf\n", spearmanCorrelationCoefficient);
    } else {
        printf("Enter sets of point in the format x,y\tBreak the rule to exit\n");
        while ((scanRes = scanf("%lf,%lf", &pair.x, &pair.y)) == 2) {
            pairPtr = realloc(pairPtr, (n + 1) * sizeof(Pair));
            xData = realloc(xData, (n + 1) * sizeof(float));
            yData = realloc(yData, (n + 1) * sizeof(float));
            if (pairPtr == NULL) printf("Pair struct was not succesfully allocated\n");
            (pairPtr + n)->x = pair.x;
            *(xData + n) = pair.x;
            (pairPtr + n)->y = pair.y;
            *(yData + n) = pair.y;
            n++;
        }
        for (size_t i = 0; i < n; i++) {
            printf("%.4lf\t%.4lf\n", (pairPtr + i)->x, (pairPtr + i)->y);
        }
        double pearsonCorrelationCoefficient = calcPearsonsCorrelationCoefficient(pairPtr, n);
        double spearmanCorrelationCoefficient = calculateSpearmanRankCoefficient(xData, yData, n);
        printf("pearson correlation coefficient = %.4lf\nspearman rank correlation coefficient = %.4lf\n", pearsonCorrelationCoefficient, spearmanCorrelationCoefficient);
        free(pairPtr);
        free(xData);
        free(yData);
    }
    return EXIT_SUCCESS;
}

RankPair *get_rank(float *data, size_t n) {
    RankPair *rank_and_pair = (RankPair *) malloc(sizeof(RankPair));
    Pair *pair = calloc(n, sizeof(Pair));
    pair -> x = *data;
    pair -> y = 1;
    int sortmap[n];
    *sortmap = 0;
    for (int i = 1; i < n; i++) {
        float temp = *(data + i);
        float sum = 0, nofequals = 1;
        for(int j = i;  j >= 0; j--) {
            if (j <= 0 || temp > (pair + j - 1) -> x ) {
                (pair + j) -> x = temp;
                sum += j + 1;
                float rank = sum / nofequals;
                for (int k = j; k < j + nofequals; k++) {
                    (pair + k) -> y = rank;
                }
                *(sortmap + j) = i;
                break;
            }
            (pair + j) -> x = (pair + j - 1) -> x;
            (pair + j) -> y = (pair + j - 1) -> y + 1;
            *(sortmap + j) = *(sortmap + j - 1);
            if (temp == (pair + j) -> x) {
                ++nofequals;
                sum += (pair + j) -> y;
            }
        }
    }
    Pair *unsorted_pair = calloc(n, sizeof(Pair));
    for (int i = 0; i < n; ++i) {
        (unsorted_pair + i) -> x = *(data + i);
        (unsorted_pair + sortmap[i]) -> y = (pair + i) -> y;
    }
    rank_and_pair -> ordered_pair = pair;
    rank_and_pair -> unordered_pair = unsorted_pair;
    rank_and_pair -> size = n;
    return rank_and_pair;
}
