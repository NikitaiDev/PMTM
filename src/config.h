#ifndef CONFIG_H
#define CONFIG_H

typedef struct {
    float current_threshold;
    float temperature_threshold;
} Thresholds;

typedef struct {
    char address[128];
    int port;
    int interval;
    char log_path[256];
    Thresholds thresholds;
} Config;

int load_config(const char *filename, Config *config);

#endif
