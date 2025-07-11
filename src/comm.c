#include "comm.h"
#include <stdio.h>
#include <string.h>

#include <fcntl.h>
#include <unistd.h>
#include <termios.h>
#include <errno.h>

static int serial_fd = -1;

int comm_open(const char *port_name, int baudrate) {
    serial_fd = open(port_name, O_RDWR | O_NOCTTY | O_NDELAY);

    if (serial_fd == -1) {
        perror("Error opening serial port");
        return -1;
    }

    struct termios options;
    tcgetattr(serial_fd, &options);

    speed_t brate = B9600; // default
    switch(baudrate) {
        case 9600: brate=B9600; break;
        case 19200: brate=B19200; break;
        case 38400: brate=B38400; break;
        case 57600: brate=B57600; break;
        case 115200: brate=B115200; break;
        default: brate=B9600; break;
    }
    cfsetispeed(&options, brate);
    cfsetospeed(&options, brate);

    options.c_cflag |= (CLOCAL | CREAD);    // enable receiver, set local mode
    options.c_cflag &= ~PARENB;             // no parity
    options.c_cflag &= ~CSTOPB;             // 1 stop bit
    options.c_cflag &= ~CSIZE;
    options.c_cflag |= CS8;                 // 8 bits

    options.c_lflag &= ~(ICANON | ECHO | ECHOE | ISIG); // raw input
    options.c_oflag &= ~OPOST;              // raw output

    tcsetattr(serial_fd, TCSANOW, &options);

    return serial_fd;
}

int comm_send(int fd, const char *data, size_t size) {
    return write(serial_fd, data, size);
}

int comm_recv(int fd, char *buf, size_t size) {
    return read(serial_fd, buf, size);
}

void comm_close(int fd) {
    if (serial_fd >= 0) {
        close(serial_fd);
        serial_fd = -1;
    }
}

