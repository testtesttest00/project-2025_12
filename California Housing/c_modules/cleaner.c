#ifndef CLEANER_C
#define CLEANER_C

#include "stats.c"


HouseStore *cleanerStd(HouseStore *h, HouseCol c, double k) {
    double *criteria = dataExtract(h, c);
    size_t count = h->count;
    double mean = myAvg(criteria, count);
    double sd = mySD(criteria, count, mean);
    double lowerLimit = mean - k * sd;
    double upperLimit = mean + k * sd;

    size_t writecount = 0;
    for (size_t i = 0; i < count; i++) {
        House *cur = h->items[i];
        if (criteria[i] >= lowerLimit && criteria[i] <= upperLimit) {
            h->items[writecount++] = cur;
        } else {
            free(cur->ocean_proximity);
            free(cur);
        }
    }
    h->items = realloc(h->items, sizeof(House *) * writecount);
    h->count = writecount;

    free(criteria);
    return h;
}


int writecsv(HouseStore *store, char *filename) {
    if (!store || store->count == 0) return 1;

    FILE *out_f = fopen(filename, "w");
    if (!out_f) return 1;

    fprintf(out_f,
        "longitude,latitude,housing_median_age,total_rooms,total_bedrooms,"
        "population,households,median_income,median_house_value,house_value_by_income,ocean_proximity\n"
    );

    // Price regression by income
    double *lin_reg = linReg(store, MEDIAN_INCOME, MEDIAN_HOUSE_VALUE);
    double a = lin_reg[0], b = lin_reg[1];
    free(lin_reg);

    for (size_t i = 0; i < store->count; i++) {
        House *h = store->items[i];

        fprintf(out_f,
            "%.2f,%.2f,%.1f,%.1f,%.1f,%.1f,%.1f,%.4f,%.1f,%.1f,%s",
            h->longitude,
            h->latitude,
            h->housing_median_age,
            h->total_rooms,
            h->total_bedrooms,
            h->population,
            h->households,
            h->median_income,
            h->median_house_value,
            a*h->median_income+b,
            h->ocean_proximity
        );
    }

    fclose(out_f);
    return 0;
}

#endif
