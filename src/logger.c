#include "logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <errno.h>

static FILE *log_file = NULL;

static void ensure_log_dir(const char *path) {
    char dir[256];
    strcpy(dir, path);

    // Обрезаем до последнего /
    char *last_slash = strrchr(dir, '/');
    if (last_slash) {
        *last_slash = '\0';
        struct stat st = {0};
        if (stat(dir, &st) == -1) {
            if (mkdir(dir, 0777) != 0) {
                perror("mkdir failed");
                exit(EXIT_FAILURE);
            }
        }
    }
}

int logger_init(const char *filename) {
    ensure_log_dir(filename);

    log_file = fopen(filename, "a");
    if (!log_file) {
        perror("Failed to open log file");
        return -1;
    }
    return 0;
}

void log_timestamp(void) {
    time_t now = time(NULL);
    struct tm *t = localtime(&now);
    char buf[64];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", t);
    fprintf(log_file, "%s ", buf);
    printf("%s ", buf);
}

void logger_data(const TelemetryData *data) {
    log_timestamp();
    fprintf(log_file, "[DATA] VOLT:%.2f CURR:%.2f TEMP:%.2f\n",
            data->voltage, data->current, data->temperature);
    printf("[DATA] VOLT:%.2f CURR:%.2f TEMP:%.2f\n",
           data->voltage, data->current, data->temperature);
    fflush(log_file);
}

void logger_alert(const char *message) {
    log_timestamp();
    fprintf(log_file, "[ALERT] %s\n", message);
    printf("[ALERT] %s\n", message);
    fflush(log_file);
}

void logger_close(void) {
    if (log_file) {
        fclose(log_file);
        log_file = NULL;
    }
}

