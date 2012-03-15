#ifndef __CLIENT_H__
#define __CLIENT_H__

#define MAX_MSG_SIZE 1024

class Client {
public:
  int id;
  int fd;

  Client(int id, int fd);

  void handle();

  void send(void *data, unsigned int size);

private:
  static char recvBuf[MAX_MSG_SIZE];

  static int recvLen;
};

#endif
