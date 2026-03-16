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
    RankPair *insertion_sort(float *data, size_t n);
    RankPair *rank_and_pair = insertion_sort((float *)xyPair, n);
    return rank_and_pair -> unordered_pair;
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

    RankPair *insertion_sort(float *data, size_t n);

    Pair *pairPtr = NULL;
    Pair pair;
    size_t n = 0;
    int scanRes;
    /*for (int i = 0; i < argc; i++) {
        printarg(argv[i]);
    }*/
    /*printf("Enter sets of point in the format x,y\tBreak the rule to exit\n");
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
    free(pairPtr);*/

    float data[] = {94, 86, 74, 21, 86, 75, 86};
    RankPair *rankPair = insertion_sort(data, 7);
    /*printf("%-10s%-10s\n", "Data", "Rank");
    for (int i = 0; i < rankPair -> size; ++i) {
        printf("%-10.2f%-10.2f\n", ((rankPair->ordered_pair) + i) -> x, ((rankPair->ordered_pair) + i) -> y);
    }*/
    printf("%-10s%-10s\n", "Data", "Rank");
    for (int i = 0; i < rankPair -> size; ++i) {
        printf("%-10.2f%-10.2f\n", ((rankPair->unordered_pair) + i) -> x, ((rankPair->unordered_pair) + i) -> y);
    }
    free(rankPair->ordered_pair);
    free(rankPair->unordered_pair);
    free(rankPair);
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

RankPair *insertion_sort(float *data, size_t n) {
    RankPair *rank_and_pair = malloc(sizeof(RankPair));
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
