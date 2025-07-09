#ifndef PARSER_H
#define PARSER_H

#include "analyzer.h"

int parse_telemetry(const char *response, TelemetryData *data);

#endif
