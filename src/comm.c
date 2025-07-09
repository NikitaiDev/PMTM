#include "comm.h"
#include <stdio.h>
#include <string.h>

#ifdef _WIN32
#include <windows.h>

static HANDLE hSerial = INVALID_HANDLE_VALUE;

int comm_open(const char *port_name, int baudrate) {
    hSerial = CreateFileA(port_name, GENERIC_READ | GENERIC_WRITE, 0, NULL,
                          OPEN_EXISTING, 0, NULL);
    if (hSerial == INVALID_HANDLE_VALUE) {
        fprintf(stderr, "Error opening COM port %s\n", port_name);
        return -1;
    }

    DCB dcbSerialParams = {0};
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    if (!GetCommState(hSerial, &dcbSerialParams)) {
        fprintf(stderr, "Error getting COM state\n");
        return -1;
    }

    dcbSerialParams.BaudRate = baudrate;
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = ONESTOPBIT;
    dcbSerialParams.Parity   = NOPARITY;

    if (!SetCommState(hSerial, &dcbSerialParams)) {
        fprintf(stderr, "Error setting COM state\n");
        return -1;
    }

    COMMTIMEOUTS timeouts = {0};
    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;
    SetCommTimeouts(hSerial, &timeouts);

    return 0;
}

int comm_send(int fd, const char *data, size_t size) {
    DWORD bytes_written;
    if (!WriteFile(hSerial, data, (DWORD)size, &bytes_written, NULL)) {
        fprintf(stderr, "Error writing to COM\n");
        return -1;
    }
    return bytes_written;
}

int comm_recv(int fd, char *buf, size_t size) {
    DWORD bytes_read;
    if (!ReadFile(hSerial, buf, (DWORD)size, &bytes_read, NULL)) {
        fprintf(stderr, "Error reading from COM\n");
        return -1;
    }
    return bytes_read;
}

void comm_close(int fd) {
    if (hSerial != INVALID_HANDLE_VALUE) {
        CloseHandle(hSerial);
        hSerial = INVALID_HANDLE_VALUE;
    }
}

#else // Linux/Unix

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

#endif
