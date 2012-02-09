#include <pron.hpp>
#include <cstdio>
extern "C" {
#include <tsock.h>
}
#define MAX_CLIENTS 128
#define MAX_MSG_SIZE 1024

Client clients[MAX_CLIENTS];
int nbClients = 0;

void handleClientRequest(int client, void *buf, int size) {
	int reqType = *((int*)buf);
	switch (reqType) {
		case RQ_WELCOME:
			printf("Received RQ_WELCOME from client %d!\n", client);
			break;
		case RQ_CREATE_WINDOW:
			printf("Received RQ_CREATE_WINDOW from client %d!\n", client);
			break;
		case RQ_MAP_WINDOW:
			printf("Received RQ_MAP_WINDOW from client %d!\n", client);
			break;
		case RQ_CREATE_GC:
			printf("Received RQ_CREATE_GC from client %d!\n", client);
			break;
		case RQ_DRAW_LINE:
			printf("Received RQ_DRAW_LINE from client %d!\n", client);
			break;
	}
}

int main() {
	int fd = tsock_listen("pron.sock");
	int continuer = 1;
	int newFd;
	int client;
	int lRead;
	char buf[MAX_MSG_SIZE];

	while (continuer) {
		// Try to accept client
		if ((newFd = tsock_accept(fd)) > 0) {
			clients[nbClients].fd = newFd;
			nbClients++;
		}

		// Read requests from clients
		for (client = 0; client < nbClients; client++) {
			if ((lRead = tsock_read(clients[client].fd, buf, sizeof(buf))) > 0) {
				handleClientRequest(client, buf, lRead);
			}
		}
	}

	tsock_close(fd);
}
