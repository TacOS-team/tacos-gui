#ifndef TSOCK_H
#define TSOCK_H

#include <sys/types.h>
#include <sys/socket.h>


int tsockListen (const struct sockaddr * servAddr,struct sockaddr * clientAddr, socklen_t servAddrLen);
    
void tsockConnect();

void tsockAccept();

void tsockRead();

void tsockWrite();

void tsockClose();

#endif
