#ifndef TSOCK_H
#define TSOCK_H

#include <sys/types.h>

int tsock_listen(char *path);
    
int tsock_connect(char *path);

int tsock_accept(int tsockServer);

ssize_t tsock_read(int tsock, void *buffer, size_t len);

ssize_t tsock_write(int tsock, void *buffer, size_t len);

void tsock_close();

#endif
