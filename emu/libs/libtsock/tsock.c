#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>

#include "tsock.h"

void setNonBlockMode(int tsock) {
    int flags = 0;
    flags = fcntl(tsock, F_GETFL);
    if (flags < 0) {
	perror ("olol problème : ");
    }
    else {
	flags += O_NONBLOCK;
    }
    fcntl(tsock, F_SETFL,flags);
}

int tsock_listen(char *path) {
    int tsockServ = 0;
    struct sockaddr_un addr;
    socklen_t addrLen = 0;

    // Paramétrage de l'adresse
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path, path);

    addrLen = sizeof(addr);

    // Création de la socket
    tsockServ = socket(PF_UNIX, SOCK_SEQPACKET, 0);
    if (tsockServ == -1) {
	perror("socket : revoyez les entrées/sorties de votre programme");
    }

    setNonBlockMode(tsockServ);

    // Bind
    if (bind(tsockServ, (struct sockaddr*) &addr, addrLen) != 0) {
	perror("bind : revoyez les entrées/sorties de votre programme");
    }

    // Listen
    if (listen(tsockServ, 5) != 0) {
	perror("listen : revoyez les entrées/sorties de votre programme");
    }

    return tsockServ;
}

int tsock_connect(char *path) {
    int tsockClient = 0;
    struct sockaddr_un addr;
    socklen_t addrLen = 0;

    // Paramétrage de l'adresse 
    memset(&addr, 0, sizeof(addr));
    addr.sun_family = AF_UNIX;
    strcpy(addr.sun_path,path);

    addrLen = sizeof(addr);

    // Création de la socket
    tsockClient = socket(PF_UNIX, SOCK_SEQPACKET, 0);
    if (tsockClient == -1) {
	perror("socket : revoyez les entrées/sorties de votre programme");
    }
    
    setNonBlockMode(tsockClient);

    // Connect
    if (connect(tsockClient, (struct sockaddr*) &addr, addrLen) != 0) {
	perror("connect : revoyez les entrées/sorties de votre programme");
    }

    return tsockClient;
}

int tsock_accept(int tsockServer) {
    struct sockaddr_un addrClientOSEF;
    socklen_t addrLenOSEF = sizeof(addrClientOSEF);
    int tsockCanal = 0; // socket du canal client <--> serveur

    tsockCanal = accept(tsockServer, (struct sockaddr*) &addrClientOSEF, &addrLenOSEF);
    /*if (tsockCanal == -1) {
	perror("accept : revoyez les entrées/sorties de votre programme");
    }*/

    return tsockCanal;
}

ssize_t tsock_read(int tsock, void *buffer, size_t len) {
    ssize_t nbBytesRcvd = recv(tsock, buffer, len, 0);

    if (nbBytesRcvd == -1) {
	perror("read : revoyez les entrées/sorties de votre programme");
    }

    return nbBytesRcvd;
}

ssize_t tsock_write(int tsock, void * buffer, size_t len) {
    ssize_t nbBytesSent = send(tsock, buffer, len, 0);

    if (nbBytesSent == -1) {
	perror("write : revoyez les entrées/sorties de votre programme");
    }

    return nbBytesSent;
}

void tsock_close(int tsock) {
    if (close(tsock) != 0) {
	perror("close : revoyez les entrées/sorties de votre programme");
    }
}
