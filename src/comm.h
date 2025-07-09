#ifndef COMM_H
#define COMM_H

#include <stddef.h>

int comm_open(const char *port_name, int baudrate);
int comm_send(int fd, const char *data, size_t size);
int comm_recv(int fd, char *buf, size_t size);
void comm_close(int fd);

#endif
