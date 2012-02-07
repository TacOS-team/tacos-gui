#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>

#include "tsock.h"


int tsockListen (char * path){
    int tsockServ = 0;
    struct sockaddr_un addr;
    socklen_t addrLen = 0;

    // Paramétrage de l'adresse
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path,path);

    addrLen = sizeof(addr);

    // Creation de la socket
    tsockServ = socket (AF_UNIX,SOCK_DGRAM,0);
    if (tsockServ == -1) {
	perror("socket : revoyez les entrees sorties de votre programme ");
    }

    // Bind
    if (bind (tsockServ, (struct sockaddr *) &addr, addrLen) != 0) {
	perror("bind : revoyez les entrees sorties de votre programme ");
    }

    // listen
    if (listen (tsockServ, 5) != 0) {
	perror("listen : revoyez les entrees sorties de votre programme ");
    }

    return tsockServ;
}

int tsockConnect(char * path) {
    int tsockClient = 0;
    struct sockaddr_un addr;
    socklen_t addrLen = 0;

    // Paramétrage de l'adresse 
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path,path);

    addrLen = sizeof(addr);

    // Création de la socket
    tsockClient = socket (AF_UNIX,SOCK_DGRAM,0);
    if (tsockClient == -1){ 
	perror("socket : revoyez les entrees sorties de votre programme ");
    }

    // connect
    if (connect (tsockClient, (struct sockaddr *) &addr, addrLen) != 0 ) {
	perror("connect : revoyez les entrees sorties de votre programme ");
    }

    return tsockClient;
}

int tsockAccept(int tsockServer) {
    struct sockaddr_un addrClientOSEF;
    socklen_t addrLenOSEF = sizeof(addrClientOSEF);
    int tsockCanal = 0; // socket du canal client <--> serveur

    tsockCanal = accept(tsockServer, (struct sockaddr *) &addrClientOSEF, &addrLenOSEF);
    if (tsockCanal == -1) {
	perror("accept : revoyez les entrees sorties de votre programme ");
    }

    return tsockCanal;
}

ssize_t tsockRead(int tsock, void * buffer, size_t len) {
    ssize_t nbBytesRcvd = recv (tsock, buffer, len, 0);
    if (nbBytesRcvd == -1) {
	perror("read : revoyez les entrees sorties de votre programme ");
    }
    return nbBytesRcvd;
}

ssize_t tsockWrite(int tsock, void * buffer, size_t len){
    ssize_t nbBytesSent = send(tsock,buffer,len,0);
    if (nbBytesSent == -1) {
	perror("write : revoyez les entrees sorties de votre programme ");
    }
    return nbBytesSent;
}

void tsockClose(int tsock) {
    if (shutdown(tsock,2) != 0) {
	perror("close : revoyez les entrees sorties de votre programme ");
    }
}
