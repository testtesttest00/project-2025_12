#ifndef ANALYTICS_C
#define ANALYTICS_C

#include <math.h> // (Compile with "-lm" cmd-line arg)
#include "stats.c"

double pearson(HouseStore *h, HouseCol x, HouseCol y);
double *linReg(HouseStore *h, HouseCol x, HouseCol y); // y = ax + b, rval = {a, b}


double pearson(HouseStore *h, HouseCol x, HouseCol y) {
    double *x_vals = dataExtract(h, x);
    double *y_vals = dataExtract(h, y);
    int count = h->count;

    double means_x = myAvg(x_vals, count);
    double means_y = myAvg(y_vals, count);

    double summation1 = 0, summation2 = 0, summation3 = 0;

    for (size_t i = 0; i < count; i++){summation1 = summation1 + (x_vals[i] - means_x)*(y_vals[i] - means_y);}
    for (size_t i = 0; i < count; i++){summation2 = summation2 + pow(x_vals[i] - means_x, 2);}
    for (size_t i = 0; i < count; i++){summation3 = summation3 + pow(y_vals[i] - means_y, 2);}

    free(x_vals);
    free(y_vals);

    return summation1 / sqrt(summation2 * summation3);
}

double *linReg(HouseStore *h, HouseCol x, HouseCol y){
    double *x_vals = dataExtract(h, x);
    double *y_vals = dataExtract(h, y);
    int count = h->count;

    double means_x = myAvg(x_vals, count);
    double means_y = myAvg(y_vals, count);

    double summation1 = 0, summation2 = 0;

    for (size_t i = 0; i < count; i++){summation1 = summation1 + (x_vals[i] - means_x)*(y_vals[i] - means_y);}
    for (size_t i = 0; i < count; i++){summation2 = summation2 + (x_vals[i] - means_x)*(x_vals[i] - means_x);}

    free(x_vals);
    free(y_vals);

    double *results = malloc(2 * sizeof(double));
    results[0] = summation1 / summation2;
    results[1] = means_y - results[0] * means_x;

    return results;
}

#endif
