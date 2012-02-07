#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>

#include "tsock.h"


int tsockListen (char * path){
    int tsockServ = 0;
    struct sockaddr_un addr;
    socklen_t addrLen = sizeof(struct sockaddr_un);

    // Paramétrage de l'adresse
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path,path);
    
    // Creation de la socket
    // d'aprèsle man SOCK_SEQPACKET préserve aussi l'ordre des paquets (et pas SOCK_DGRAM)
    tsockServ = socket (AF_UNIX,SOCK_SEQPACKET,0);
    if (tsockServ == -1) {
	perror("Revoyez les entrees sorties de votre programme ");
    }

    // Bind
    if (! bind (tsockServ, (struct sockaddr *) &addr, addrLen) ) {
	perror("Revoyez les entrees sorties de votre programme ");
    }

    // listen
    if (! listen (tsockServ, 5)) {
	perror("Revoyez les entrees sorties de votre programme ");
    }

    return tsockServ;
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
