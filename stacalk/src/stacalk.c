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

RankPair *get_rank(double *data, size_t size);
Pair *convert_to_pair(double *x_val, double *y_val, size_t size);

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

/**
 * Calculates the Pearson correlation coefficient based on an array of x,y value pairs and size of the pairs
 */
double calcPearsonsCorrelationCoefficient(Pair *xyPair, size_t size)
{
    double sumX, sumY, sumXSquared, sumYSquared, sumXY;
    double ssxy, ssxx, ssyy;

    for (int i = 0; i < size; i++) {
        sumX += (xyPair + i)->x;
        sumY += (xyPair + i)->y;
        sumXSquared += pow((xyPair + i)->x, 2);
        sumYSquared += pow((xyPair + i)->y, 2);
        sumXY += (xyPair + i)->x * (xyPair + i)->y;
    }
    ssxy = calcSSxy(size, sumXY, sumX, sumY);
    ssxx = calcSSxx(size, sumXSquared, sumX);
    ssyy = calcSSyy(size, sumYSquared, sumY);
    return ssxy/sqrt(ssxx * ssyy);
}

double calculateSpearmanRankCoefficient(double *xData, double *yData, size_t n) 
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
    free(xrank->ordered_pair);
    free(xrank->unordered_pair);
    free(xrank);
    free(yrank->ordered_pair);
    free(yrank->unordered_pair);
    free(yrank);
    return 1 - (6 * rank_diff_squared) / (pow(n, 3) - n);
}

int main (int argc, char **argv) {
    Pair *pairPtr = NULL;
    double *xData = NULL;
    double *yData = NULL;
    Pair pair;
    size_t size = 0;
    int scanRes;

    for (int i = 0; i < argc; i++) {
        printf("arg: %s,\t length: %zu,\t type: %s\n", argv[i], strlen(argv[i]), (*argv[i] == '-' ? "option" : "command"));
    }

    printf("Use test values: (Y)/N: ");
    int c = getchar();
    if (c == 'Y' || c == 'y' || c == "\n"){
        double xval[10] = {75, 80, 93, 65, 87, 71, 98, 68, 84, 77};
        double yval[10] = {82, 78, 86, 72, 91, 80, 95, 72, 89, 74};
        
        size_t length = sizeof (xval) / sizeof (*xval);

        pairPtr = convert_to_pair(xval, yval, length);
        double pearsonCorrelationCoefficient = calcPearsonsCorrelationCoefficient(pairPtr, length);
        double spearmanCorrelationCoefficient = calculateSpearmanRankCoefficient(xval, yval, length);
        printf("\npearson correlation coefficient = %.4lf\nspearman rank correlation coefficient = %.4lf\n", pearsonCorrelationCoefficient, spearmanCorrelationCoefficient);
    } else {
        printf("Enter sets of point in the format x,y\tBreak the rule to exit\n");
        while ((scanRes = scanf("%lf,%lf", &pair.x, &pair.y)) == 2) {
            pairPtr = realloc(pairPtr, (size + 1) * sizeof(Pair));
            xData = realloc(xData, (size + 1) * sizeof(float));
            yData = realloc(yData, (size + 1) * sizeof(float));
            if (pairPtr == NULL) printf("Pair struct was not succesfully allocated\n");
            (pairPtr + size)->x = pair.x;
            *(xData + size) = pair.x;
            (pairPtr + size)->y = pair.y;
            *(yData + size) = pair.y;
            size++;
        }
        for (size_t i = 0; i < size; i++) {
            printf("%.4lf\t%.4lf\n", (pairPtr + i)->x, (pairPtr + i)->y);
        }
        double pearsonCorrelationCoefficient = calcPearsonsCorrelationCoefficient(pairPtr, size);
        double spearmanCorrelationCoefficient = calculateSpearmanRankCoefficient(xData, yData, size);
        printf("pearson correlation coefficient = %.4lf\nspearman rank correlation coefficient = %.4lf\n", pearsonCorrelationCoefficient, spearmanCorrelationCoefficient);
        free(xData);
        free(yData);
    }
    free(pairPtr);
    return EXIT_SUCCESS;
}

RankPair *get_rank(double *data, size_t n) {
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

Pair *convert_to_pair(double *x_val, double *y_val, size_t size) 
{
    Pair *pairPtr;
    size_t x_length = sizeof (x_val) / sizeof (*x_val);
    size_t y_length = sizeof y_val / sizeof *y_val;
    if (x_length > y_length) {
        puts("There are more x values than y values.");
        puts("Proceeding with available complete pairs");
    } else if (x_length < y_length) {
        puts("There are more y values than x values.");
        puts("Proceeding with available complete pairs");
    }
    
    pairPtr = malloc(size * sizeof (Pair));
    for (size_t i = 0; i < size; i++) {
        (pairPtr + i)->x = x_val[i];
        (pairPtr + i)->y = y_val[i];
    }
    return pairPtr;
}
