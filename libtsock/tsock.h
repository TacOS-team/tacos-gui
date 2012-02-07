#ifndef TSOCK_H
#define TSOCK_H

#include <sys/types.h>

int tsockListen (char * path);
    
int tsockConnect(char * path) ;

int tsockAccept(int tsockServer) ;

ssize_t tsockRead(int tsock, void * buffer, size_t len) ;

ssize_t tsockWrite(int tsock, void * buffer, size_t len);

void tsockClose();

#endif
