#ifndef __CLIENT_H__
#define __CLIENT_H__

/**
 * @file client.h
 * Client class definition.
 */

#define MAX_MSG_SIZE 1024 /**< Maximum message size (1Kio) */

/**
 * Client connected to pron.
 */
class Client {
public:
  int id; /**< Client id */
  int fd; /**< File descriptor used to communicate with the client */

  /**
   * Constructor.
   * @param id The id of the client
   * @param fd The file descriptor used to communicate with the client
   */
  Client(int id, int fd);

  /**
   * Reads from the client and handles new messages he has sent.
   */
  void handle();

  /**
   * Sends a message to the client.
   * @param data The message buffer
   * @param size The size of the message
   */
  void send(void *data, unsigned int size);

private:
  static char recvBuf[MAX_MSG_SIZE]; /**< Buffer that temporary stores new messages received from this client */
  static int recvLen; /**< Length of the received message */
};

#endif
