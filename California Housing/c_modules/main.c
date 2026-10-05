#include <string.h>
#include "stats.c"
#include "analytics.c"
#include "cleaner.c"


int main(int argc, char *argv[]) {
    int json = 0;
    int clean = 0;
    for(size_t i = 0; i < argc; i++) {
        if(strcmp(argv[i], "--json") == 0) {json = 1; continue;}
        if(strcmp(argv[i], "--clean") == 0) {clean = 1; continue;}
    }

    HouseStore *list = parse();
    if (!list) {
        fprintf(stderr, "parse() failed\n");
        return 1;
    }
    if(clean) {list = cleanerStd(list, MEDIAN_HOUSE_VALUE, 3.0); list = cleanerStd(list, MEDIAN_INCOME, 3.0); writecsv(list, "../assets/housing_clean.csv");}

    double *priceStats = printStats(list, MEDIAN_HOUSE_VALUE, !json);
    double *incomeStats = printStats(list, MEDIAN_INCOME, !json);
    double pearsons = pearson(list, MEDIAN_INCOME, MEDIAN_HOUSE_VALUE);
    double *lin_reg = linReg(list, MEDIAN_INCOME, MEDIAN_HOUSE_VALUE);
    printf("Pearson value: %f\n", pearsons);
    printf("Price = %f * Income + (%f)\n", lin_reg[0], lin_reg[1]);
    if(json) {
        printf("\n");
        printf("{\"price\":{\"mn\":%f, \"sd\":%f, \"md\":%f}, \"income\":{\"mn\":%f, \"sd\":%f, \"md\":%f}, \"pearson\":%f, \"ax+b\":{\"a\":%f, \"b\":%f}}",
            priceStats[0], priceStats[1], priceStats[2],
            incomeStats[0], incomeStats[1], incomeStats[2],
            pearsons,
            lin_reg[0], lin_reg[1]
        );
        printf("\n");
    }
    free(priceStats);
    free(incomeStats);
    free(lin_reg);

    demolish(list);
    return 0;
}
