#ifndef STATS_C
#define STATS_C

#include <math.h> // (Compile with "-lm" cmd-line arg)
#include <string.h>
#include "parse.c"

// House Prices ----
// Central Tendency & Dispersion; Mean, Median, SD

double *dataExtract(HouseStore *list, HouseCol col); // Convert a list of house data into a list of doubles
double *mySorter(double *prices, int count); // Sort the array of prices in ascending order
double myMed(double *values, int count); // Get the median value from sorted array (for median)
double myAvg(double *values, int count); // Get the average value (for mean, even-set median)
double mySD(double *values, int count, double avg); // Get the standard deviation (for SD)


double *printStats(HouseStore *h, HouseCol c, int print) {
    double mean = 0, med = 0, sd = 0;
    double *criterion = dataExtract(h, c);
    int count = h->count;

    double *sorted_c = mySorter(criterion, count);
    free(criterion);

    mean = myAvg(sorted_c, count);
    med = myMed(sorted_c, count);
    sd = mySD(sorted_c, count, mean);
    free(sorted_c);
    
    if (print) {
        printf("\
            ==%s==\n\
            Mean           %.2f\n\
            SD             %.2f\n\
            Median         %.2f\n\
            ========================\n",
            HouseColNames[c], mean, sd, med
        );
        printf("\n");
    }

    double *package = malloc(sizeof(double) * 3);
    package[0] = mean;
    package[1] = sd;
    package[2] = med;
    return package;
}


double *dataExtract(HouseStore *list, HouseCol col) {
    double *column = malloc(sizeof(double)*list->count);
    for(size_t i = 0; i < list->count; i++) {
        switch(col) {
            case(LONGITUDE): column[i] = list->items[i]->longitude; break;
            case(LATITUDE): column[i] = list->items[i]->latitude; break;
            case(HOUSING_MEDIAN_AGE): column[i] = list->items[i]->housing_median_age; break;
            case(TOTAL_ROOMS): column[i] = list->items[i]->total_rooms; break;
            case(TOTAL_BEDROOMS): column[i] = list->items[i]->total_bedrooms; break;
            case(POPULATION): column[i] = list->items[i]->population; break;
            case(HOUSEHOLDS): column[i] = list->items[i]->households; break;
            case(MEDIAN_INCOME): column[i] = list->items[i]->median_income; break;
            case(MEDIAN_HOUSE_VALUE): column[i] = list->items[i]->median_house_value; break;
            case(OCEAN_PROXIMITY): free(column); return NULL; // Non-numeric exception
        }
    }
    return column;
}


double *mySorter(double *prices, int count) {
    // Quick-sort recursion
    if (count <= 1) {
        double *base = malloc(count * sizeof(double));
        if (count == 1) base[0] = prices[0];
        return base;
    }

    double pivot_val = prices[count - 1];
    double *lower_list = NULL;
    double *upper_list = NULL;
    size_t lower = 0;
    size_t upper = 0;

    for(size_t i = 0; i < count - 1; i++) {
        if (prices[i] < pivot_val) {
            lower_list = realloc(lower_list, (lower + 1) * sizeof(double));
            lower_list[lower] = prices[i];
            lower++;
        } else {
            upper_list = realloc(upper_list, (upper + 1) * sizeof(double));
            upper_list[upper] = prices[i];
            upper++;
        }
    }

    double *sorted = malloc(count * sizeof(double));
    if (lower) {
        double *sorted_lower = mySorter(lower_list, lower);
        memcpy(sorted, sorted_lower, lower * sizeof(double));
        free(sorted_lower);
    }
    memcpy(sorted + lower, &pivot_val, sizeof(double));
    if (upper) {
        double *sorted_upper = mySorter(upper_list, upper);
        memcpy(sorted + lower + 1, sorted_upper, upper * sizeof(double));
        free(sorted_upper);
    }
    free(lower_list);
    free(upper_list);

    return sorted;
}


double myMed(double *values, int count) {
    if (count % 2 == 1) return values[count/2];
    return (values[count/2-1] + values[count/2]) / 2.0;
}


double myAvg(double *values, int count) {
    double sum = 0;
    for(size_t i = 0; i < count; i++){
        sum = sum + values[i];
    }
    return (sum / count);
}


double mySD(double *values, int count, double avg) {
    double rolling = 0; // sum((x-m)^2)
    for(size_t i = 0; i < count; i++){
        rolling = rolling + pow(values[i] - avg, 2);
    }
    return (sqrt(rolling / count));
}

#endif
