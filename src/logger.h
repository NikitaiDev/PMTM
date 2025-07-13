#ifndef LOGGER_H
#define LOGGER_H

#include "analyzer.h"

int logger_init(const char *filename);
void logger_data(const TelemetryData *data);
void logger_alert(const char *message);
void logger_close(void);

#endif
