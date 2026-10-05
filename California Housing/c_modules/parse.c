#ifndef PARSE_C
#define PARSE_C

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>
#include "house.h"

char *IN_FILE = "../assets/housing.csv";

House *builder(char *s);
int demolish(HouseStore *h);


HouseStore *parse(void) {
    clock_t begin = clock();  // Start parsing
    FILE *in_f = fopen(IN_FILE, "r");
    if (!in_f) {perror("fopen failed"); return NULL;}

    HouseStore *store = malloc(sizeof(HouseStore));
    store->items = NULL;
    store->count = 0;

    char line_content[LINE_SIZE];
    fgets(line_content, LINE_SIZE, in_f);  // Run once to discard header row
    while (fgets(line_content, LINE_SIZE, in_f)) {
        House *h = builder(line_content);
        if (!h) continue;  // Skip bad rows

        store->items = realloc(store->items, (store->count + 1) * sizeof(House *));
        store->items[store->count] = h;
        store->count ++;
    }
    fclose(in_f);  // End parsing

    clock_t end = clock();
    double time_spent = (double)(end - begin) / CLOCKS_PER_SEC;
    printf("\n## csv file was read in %f seconds ##\n\n", time_spent);

    return store;
}


House *builder(char *s) {
    // House has 9 double, 1 char* in order
    House *h = malloc(sizeof(House));
    char *last = NULL;

    double *fields[] = {
        &h->longitude,
        &h->latitude,
        &h->housing_median_age,
        &h->total_rooms,
        &h->total_bedrooms,
        &h->population,
        &h->households,
        &h->median_income,
        &h->median_house_value
    };

    for(size_t index = 0; ; index++, s = NULL) {
        char *tok = strtok_r(s, ",", &last);
        if (!tok) {free(h); return NULL;}  // Unclean rows are fully removed

        if (index < 9) {
            *(fields[index]) = strtod(tok, NULL);
        } else if (index == 9) {
            h->ocean_proximity = strdup(tok);
            break;
        }
    }

    return h;
}


int demolish(HouseStore *h) {
    for (size_t i = 0; i < h->count; i++) {
        free(h->items[i]->ocean_proximity);
        free(h->items[i]);
    }
    free(h->items);
    free(h);
    return 0;
}

#endif
