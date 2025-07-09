#include "config.h"
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "cJSON.h"

int load_config(const char *filename, Config *config) {
    FILE *fp = fopen(filename, "rb");
    if (!fp) {
        perror("Cannot open config file");
        return -1;
    }

    fseek(fp, 0, SEEK_END);
    long len = ftell(fp);
    rewind(fp);

    char *data = malloc(len + 1);
    if (!data) {
        fclose(fp);
        fprintf(stderr, "Out of memory reading config\n");
        return -1;
    }
    fread(data, 1, len, fp);
    data[len] = '\0';
    fclose(fp);

    cJSON *root = cJSON_Parse(data);
    free(data);
    if (!root) {
        fprintf(stderr, "Error parsing config JSON\n");
        return -1;
    }

    const cJSON *addr = cJSON_GetObjectItemCaseSensitive(root, "address");
    const cJSON *port = cJSON_GetObjectItemCaseSensitive(root, "port");
    const cJSON *interval = cJSON_GetObjectItemCaseSensitive(root, "interval");
    const cJSON *log_path = cJSON_GetObjectItemCaseSensitive(root, "log_path");
    const cJSON *thresholds = cJSON_GetObjectItemCaseSensitive(root, "thresholds");

    if (!cJSON_IsString(addr) || !cJSON_IsNumber(port) || !cJSON_IsNumber(interval) || !cJSON_IsString(log_path) || !cJSON_IsObject(thresholds)) {
        fprintf(stderr, "Invalid config format\n");
        cJSON_Delete(root);
        return -1;
    }

    strncpy(config->address, addr->valuestring, sizeof(config->address) - 1);
    config->port = port->valueint;
    config->interval = interval->valueint;
    strncpy(config->log_path, log_path->valuestring, sizeof(config->log_path) - 1);

    const cJSON *curr_thr = cJSON_GetObjectItemCaseSensitive(thresholds, "current");
    const cJSON *temp_thr = cJSON_GetObjectItemCaseSensitive(thresholds, "temperature");
    if (!cJSON_IsNumber(curr_thr) || !cJSON_IsNumber(temp_thr)) {
        fprintf(stderr, "Invalid threshold format\n");
        cJSON_Delete(root);
        return -1;
    }

    config->thresholds.current_threshold = (float)curr_thr->valuedouble;
    config->thresholds.temperature_threshold = (float)temp_thr->valuedouble;

    cJSON_Delete(root);
    return 0;
}
