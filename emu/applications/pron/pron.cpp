#include <pron.h>
#include <cstdio>
extern "C" {
#include <tsock.h>
}
#include <unistd.h>
#define MAX_CLIENTS 128
#define MAX_MSG_SIZE 1024

Client clients[MAX_CLIENTS];
int nbClients = 0;

void handleClientRequest(int client, void *buf, int size) {
	int reqType = *((int*)buf);
	switch (reqType) {
		case RQ_HELLO: {
			printf("Received RQ_HELLO from client %d!\n", client);
			RqHello *rq = (RqHello*)buf;
			printf("protoVersion = %d\n", rq->protoVersion);
			break;
		}
		case RQ_CREATE_WINDOW: {
			printf("Received RQ_CREATE_WINDOW from client %d!\n", client);
			break;
		}
		case RQ_MAP_WINDOW: {
			printf("Received RQ_MAP_WINDOW from client %d!\n", client);
			break;
		}
		case RQ_CREATE_GC: {
			printf("Received RQ_CREATE_GC from client %d!\n", client);
			break;
		}
		case RQ_DRAW_LINE: {
			printf("Received RQ_DRAW_LINE from client %d!\n", client);
			break;
		}
	}
}

int main() {
	int fd = tsock_listen("/tmp/pron.sock");
	int continuer = 1;
	int newFd;
	int client;
	int lRead;
	char buf[MAX_MSG_SIZE];

	printf("Welcome to pr0n \\o/\n");

	while (continuer) {
		// Try to accept client
		printf("Trying to accept client...\n");
		if ((newFd = tsock_accept(fd)) > 0) {
			printf("New client (%d)!\n", nbClients);
			clients[nbClients].fd = newFd;
			nbClients++;
		}

		// Read requests from clients
		printf("Reading requests from clients...\n");
		for (client = 0; client < nbClients; client++) {
			printf("Reading from client %d\n", client);
			if ((lRead = tsock_read(clients[client].fd, buf, sizeof(buf))) > 0) {
				printf("Read %d bytes from client %d\n", lRead, client);
				handleClientRequest(client, buf, lRead);
			}
		}

		usleep(1000000);
	}

	tsock_close(fd);
}
