#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>
#include <string.h>

#include "comm.h"
#include "config.h"
#include "analyzer.h"
#include "parser.h"
#include "logger.h"

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


    logger_init(config.log_path);

    printf("Config loaded: port=%s, baudrate=115200, interval=%d sec\n",
           config.address, config.interval);

    int ret = comm_open(config.address, 115200);
    if (ret < 0) {
        fprintf(stderr, "Failed to open UART %s; error code: %d\n", config.address, ret);
        return EXIT_FAILURE;
    }

    Statistics stats;
    init_statistics(&stats);

    while (keep_running) {
	sleep(config.interval);
        const char *cmd = "GET\n";
	ret = comm_send(cmd, strlen(cmd));
        if (ret < 0) {
            fprintf(stderr, "Error sending request: %i code\n", ret);
            break;
        }

        char buf[256] = {0};
        int len = comm_recv(buf, sizeof(buf) - 1);
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

	logger_data(&data);

	if (check_thresholds(&data, &config.thresholds) != 0) {
    	    logger_alert("Threshold exceeded!");
	}

    }

    comm_close(0);
    logger_close();

    printf("STM32F4 Monitor stopped\n");
    return EXIT_SUCCESS;
}
