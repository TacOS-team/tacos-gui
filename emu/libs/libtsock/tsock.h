#ifndef TSOCK_H
#define TSOCK_H


#define UNIX_PATH_MAX    108

int tsockListen (char * path);
    
void tsockConnect();

void tsockAccept();

void tsockRead();

void tsockWrite();

void tsockClose();

#endif
