#include "analyzer.h"
#include <float.h>

void init_statistics(Statistics *stats) {
    stats->min_voltage = FLT_MAX;
    stats->max_voltage = FLT_MIN;
    stats->sum_voltage = 0.0f;
    stats->min_current = FLT_MAX;
    stats->max_current = FLT_MIN;
    stats->sum_current = 0.0f;
    stats->min_temperature = FLT_MAX;
    stats->max_temperature = FLT_MIN;
    stats->sum_temperature = 0.0f;
    stats->samples = 0;
}

void update_statistics(Statistics *stats, const TelemetryData *data) {
    if (data->voltage < stats->min_voltage) stats->min_voltage = data->voltage;
    if (data->voltage > stats->max_voltage) stats->max_voltage = data->voltage;
    stats->sum_voltage += data->voltage;

    if (data->current < stats->min_current) stats->min_current = data->current;
    if (data->current > stats->max_current) stats->max_current = data->current;
    stats->sum_current += data->current;

    if (data->temperature < stats->min_temperature) stats->min_temperature = data->temperature;
    if (data->temperature > stats->max_temperature) stats->max_temperature = data->temperature;
    stats->sum_temperature += data->temperature;

    stats->samples++;
}

void compute_averages(Statistics *stats, float *avg_voltage, float *avg_current, float *avg_temperature) {
    if (stats->samples == 0) {
        *avg_voltage = *avg_current = *avg_temperature = 0.0f;
        return;
    }
    *avg_voltage = stats->sum_voltage / stats->samples;
    *avg_current = stats->sum_current / stats->samples;
    *avg_temperature = stats->sum_temperature / stats->samples;
}

int check_thresholds(const TelemetryData *data, const Thresholds *thresholds) {
    if (data->current > thresholds->current_threshold || data->temperature > thresholds->temperature_threshold) {
        return -1;
    }
    return 0;
}
