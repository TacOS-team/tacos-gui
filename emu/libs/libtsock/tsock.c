#define _GNU_SOURCE

#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

#include <tsock.h>

int tsock_listen(const char *path) {
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
    perror("socket");
  }

  // Bind
  if (bind(tsockServ, (struct sockaddr*) &addr, addrLen) != 0) {
    perror("bind");
  }

  // Listen
  if (listen(tsockServ, 5) != 0) {
    perror("listen");
  }

  return tsockServ;
}

int tsock_connect(const char *path) {
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
    perror("socket");
  }

  // Connect
  if (connect(tsockClient, (struct sockaddr*) &addr, addrLen) != 0) {
    perror("connect");
    return -1;
  }

  return tsockClient;
}

int tsock_accept(int tsockServer) {
  struct sockaddr_un addrClientOSEF;
  socklen_t addrLenOSEF = sizeof(addrClientOSEF);
  int tsockCanal = 0; // socket du canal client <--> serveur

  tsockCanal = accept(tsockServer, (struct sockaddr*) &addrClientOSEF, &addrLenOSEF);
  if (tsockCanal == -1) {
    //perror("accept");
  }

  return tsockCanal;
}

ssize_t tsock_read(int tsock, void *buffer, size_t len) {
  ssize_t nbBytesRcvd = recv(tsock, buffer, len, 0);

  if (nbBytesRcvd == -1) {
    //perror("read");
  }

  return nbBytesRcvd;
}

ssize_t tsock_write(int tsock, void * buffer, size_t len) {
  ssize_t nbBytesSent = send(tsock, buffer, len, 0);

  if (nbBytesSent == -1) {
    //perror("write");
  }

  return nbBytesSent;
}

void tsock_close(int tsock) {
  if (close(tsock) != 0) {
    perror("close");
  }
}

int tsock_set_nonblocking(int tsock) {
  int flags = fcntl(tsock, F_GETFL, 0);
  return fcntl(tsock, F_SETFL, flags | O_NONBLOCK);
}

int tsock_set_blocking(int tsock) {
  int flags = fcntl(tsock, F_GETFL, 0);
  return fcntl(tsock, F_SETFL, flags & ~O_NONBLOCK);
}
