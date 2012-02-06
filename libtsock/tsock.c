#include <stdio.h>

#include "tsock.h"


int tsockListen (const struct sockaddr * servAddr,struct sockaddr * clientAddr, socklen_t servAddrLen) {
    int tsockServ = 0;
    int tsockCanal = 0;
    socklen_t clientAddrLen = sizeof(*clientAddr);

    // Creation de la socket
    tsockServ = socket (AF_UNIX,SOCK_DGRAM,0);
    if (tsockServ == -1) {
	perror("Revoyez les entrees sorties de votre programme ");
    }

    // Bind
    if (! bind (tsockServ, servAddr, servAddrLen) ) {
	perror("Revoyez les entrees sorties de votre programme ");
    }

    // accept
    tsockCanal = accept (tsockServ, clientAddr, &clientAddrLen);


    return tsockCanal;
}

void tsockConnect() {
}

void tsockAccept() {
}

void tsockRead() {
}

void tsockWrite(){
}

void tsockClose() {
}
