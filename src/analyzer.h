#ifndef ANALYZER_H
#define ANALYZER_H

#include "config.h"

typedef struct {
    float min_voltage;
    float max_voltage;
    float sum_voltage;
    float min_current;
    float max_current;
    float sum_current;
    float min_temperature;
    float max_temperature;
    float sum_temperature;
    unsigned int samples;
} Statistics;

typedef struct {
    float voltage;
    float current;
    float temperature;
} TelemetryData;

void init_statistics(Statistics *stats);
void update_statistics(Statistics *stats, const TelemetryData *data);
void compute_averages(Statistics *stats, float *avg_voltage, float *avg_current, float *avg_temperature);
int check_thresholds(const TelemetryData *data, const Thresholds *thresholds);

#endif
