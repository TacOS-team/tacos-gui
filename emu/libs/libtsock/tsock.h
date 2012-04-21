#ifndef TSOCK_H
#define TSOCK_H

#ifdef __cplusplus
extern "C" {
#endif

/**
 * Creates a server socket listening on the given meetpoint.
 *
 * @param path Meetpoint (path on the filesystem)
 *
 * @return File descriptor number, negative in case of error
 */
int tsock_listen(const char *path);
    
/**
 * Creates a client socket connected to the server socket listening on the
 * given meetpoint.
 *
 * @param path Meetpoint
 *
 * @return File descriptor number, negative in case of error
 */
int tsock_connect(const char *path);

/**
 * Waits for a new connection on the given server socket. Creates and returns
 * a new server socket associated with the new client.
 *
 * @param tsockServer Server socket waiting for a connection
 *
 * @return File descriptor number for the new server socket,
 * negative in case of error
 */
int tsock_accept(int tsockServer);

/**
 * Reads from a socket.
 *
 * @param tsock File descriptor number for the socket
 * @param buffer Buffer where to store the data read
 * @param len Maximum size to read
 *
 * @return Number of bytes read (0 if the connection has been closed,
 * -1 in case of error)
 */
ssize_t tsock_read(int tsock, void *buffer, size_t len);

/**
 * Writes to a socket.
 *
 * @param tsock File descriptor number for the socket
 * @param buffer Buffer where to store the data read
 * @param len Maximum size to read
 *
 * @return Number of bytes written (0 if nothing written, -1 in case of error).
 */
ssize_t tsock_write(int tsock, void *buffer, size_t len);

/**
 * Closes a socket (and the associated connection).
 *
 * @param tsock File descriptor number for the socket
 *
 * @return 0 on success
 */
void tsock_close(int tsock);

/**
 * Switches a socket to nonblocking mode.
 *
 * @param tsock File descriptor number for the socket
 *
 * @return 0 on success
 */
int tsock_set_nonblocking(int tsock);

/**
 * Switches a socket to blocking mode.
 *
 * @param tsock File descriptor number for the socket
 *
 * @return 0 on success
 */
int tsock_set_blocking(int tsock);

#ifdef __cplusplus
}
#endif

#endif
