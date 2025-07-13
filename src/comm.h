#ifndef COMM_H
#define COMM_H

#include <stddef.h>

int comm_open(const char *port_name, int baudrate);
int comm_send(const char *data, size_t size);
int comm_recv(char *buf, size_t size);
void comm_close();

#endif
