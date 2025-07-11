#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

#include "comm.h"
#include "config.h"
#include "analyzer.h"
#include "parser.h"

static volatile int keep_running = 1;

void int_handler(int dummy) {
    keep_running = 0;
}

int main() {
    signal(SIGINT, int_handler);
    signal(SIGTERM, int_handler);

    printf("STM32F4 Monitor started\n");

    Config config;
    if (load_config("config/settings.ini", &config) != 0) {
        fprintf(stderr, "Failed to load config, exiting.\n");
        return EXIT_FAILURE;
    }

    printf("Config loaded: port=%s, baudrate=115200, interval=%d sec\n",
           config.address, config.interval);

    if (comm_open(config.address, 115200) != 0) {
        fprintf(stderr, "Failed to open UART %s\n", config.address);
        return EXIT_FAILURE;
    }

    Statistics stats;
    init_statistics(&stats);

    while (keep_running) {
        const char *cmd = "GET\n";
        if (comm_send(0, cmd, strlen(cmd)) < 0) {
            fprintf(stderr, "Error sending request\n");
            break;
        }

        char buf[256] = {0};
        int len = comm_recv(0, buf, sizeof(buf) - 1);
        if (len <= 0) {
            fprintf(stderr, "Error receiving response\n");
            break;
        }
        buf[len] = '\0';

        TelemetryData data;
        if (parse_telemetry(buf, &data) != 0) {
            fprintf(stderr, "Invalid telemetry data\n");
            continue;
        }

        update_statistics(&stats, &data);

        float avg_v, avg_c, avg_t;
        compute_averages(&stats, &avg_v, &avg_c, &avg_t);

        printf("[DATA] V=%.2fV, C=%.2fA, T=%.2fC | avg V=%.2f, C=%.2f, T=%.2f\n",
               data.voltage, data.current, data.temperature, avg_v, avg_c, avg_t);

        if (check_thresholds(&data, &config.thresholds) != 0) {
            printf("[ALERT] Threshold exceeded! Current: %.2f, Temperature: %.2f\n",
                   data.current, data.temperature);
        }

        sleep(config.interval);
    }

    comm_close(0);

    printf("STM32F4 Monitor stopped\n");
    return EXIT_SUCCESS;
}
