#include "parser.h"
#include <stdio.h>
#include <string.h>

int parse_telemetry(const char *response, TelemetryData *data) {
    // Ожидаем строку формата: VOLT:12.5;CURR:4.3;TEMP:55.1
    float v = 0, c = 0, t = 0;
    int ret = sscanf(response, "VOLT:%f;CURR:%f;TEMP:%f", &v, &c, &t);
    if (ret != 3) {
        fprintf(stderr, "Failed to parse telemetry: %s\n", response);
        return -1;
    }

    data->voltage = v;
    data->current = c;
    data->temperature = t;
    return 0;
}
