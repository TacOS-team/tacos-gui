#ifndef TSOCK_H
#define TSOCK_H

#include <sys/types.h>

#ifdef __cplusplus
extern "C" {
#endif

int tsock_listen(const char *path);
    
int tsock_connect(const char *path);

int tsock_accept(int tsockServer);

ssize_t tsock_read(int tsock, void *buffer, size_t len);

ssize_t tsock_write(int tsock, void *buffer, size_t len);

void tsock_close(int tsock);

#ifdef __cplusplus
}
#endif

#endif
