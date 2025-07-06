#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

static volatile int keep_running = 1;

void int_handler(int dummy) {
    keep_running = 0;
}

int main() {
    signal(SIGINT, int_handler);
    signal(SIGTERM, int_handler);

    printf("STM32F4 Monitor started\n");

    while (keep_running) {
        printf("Polling telemetry...\n");

        sleep(1);
    }

    printf("STM32F4 Monitor stopped\n");
    return EXIT_SUCCESS;
}