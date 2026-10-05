#ifndef HOUSE_H
#define HOUSE_H

#define LINE_SIZE 1024

typedef struct {
    double longitude;
    double latitude;
    double housing_median_age;
    double total_rooms;
    double total_bedrooms;
    double population;
    double households;
    double median_income;
    double median_house_value;
    char *ocean_proximity;
} House;

typedef struct {
    House **items;
    size_t count;
} HouseStore;

typedef enum {
    LONGITUDE,
    LATITUDE,
    HOUSING_MEDIAN_AGE,
    TOTAL_ROOMS,
    TOTAL_BEDROOMS,
    POPULATION,
    HOUSEHOLDS,
    MEDIAN_INCOME,
    MEDIAN_HOUSE_VALUE,
    OCEAN_PROXIMITY
} HouseCol;

static const char *HouseColNames[] = {
    "==== Longitude =====",
    "===== Latitude =====",
    " Housing Median Age ",
    "=== Total Rooms ====",
    "== Total Bedrooms ==",
    "==== Population ====",
    "==== Households ====",
    "== Median Income ===",
    " Median House Value ",
    "= Ocean Proximity =="
};

#endif
