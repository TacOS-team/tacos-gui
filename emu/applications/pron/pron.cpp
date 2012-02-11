#include <pron.h>
#include <cstdio>
extern "C" {
#include <tsock.h>
}
#include <unistd.h>
#define MAX_CLIENTS 128
#define MAX_MSG_SIZE 1024

static Screen *screen;
static Client clients[MAX_CLIENTS+1];
static int nbClients = 0;

void handleClientRequest(int client, void *buf, int size) {
	int reqType = *((int*)buf);
	switch (reqType) {
		case RQ_HELLO: {
			//printf("Received RQ_HELLO from client %d!\n", client);
			//RqHello *rq = (RqHello*)buf;
			RespWelcome welcome(screen->root->id, client << 16, (client << 17) - 1);
			tsock_write(clients[client].fd, &welcome, sizeof(welcome));
			break;
		}
		case RQ_CREATE_WINDOW: {
			//printf("Received RQ_CREATE_WINDOW from client %d!\n", client);
			RqCreateWindow *rq = (RqCreateWindow*)buf;
			Window *w = new Window(screen, rq->id, screen->getWindow(rq->parent), rq->x, rq->y, rq->width, rq->height);
			screen->addWindow(w);
			break;
		}
		case RQ_CLEAR_WINDOW: {
			//printf("Received RQ_CLEAR_WINDOW from client %d!\n", client);
			RqClearWindow *rq = (RqClearWindow*)buf;
			screen->getWindow(rq->window)->clear();
			break;
		}
		case RQ_FLUSH_WINDOW: {
			//printf("Received RQ_FLUSH_WINDOW from client %d!\n", client);
			RqFlushWindow *rq = (RqFlushWindow*)buf;
			screen->getWindow(rq->window)->flush();
			break;
		}
		case RQ_MAP_WINDOW: {
			//printf("Received RQ_MAP_WINDOW from client %d!\n", client);
			break;
		}
		case RQ_CREATE_GC: {
			//printf("Received RQ_CREATE_GC from client %d!\n", client);
			break;
		}
		case RQ_DRAW_LINE: {
			//printf("Received RQ_DRAW_LINE from client %d!\n", client);
			RqDrawLine *rq = (RqDrawLine*)buf;
			screen->getWindow(rq->drawable)->drawLine(rq->x1, rq->y1, rq->x2, rq->y2);
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

	screen = new Screen(800, 600, 24);
	screen->root = new Window(screen, 0, NULL, 0, 0, 800, 600);
	screen->addWindow(screen->root);

	while (continuer) {
		// Try to accept new client
		if ((newFd = tsock_accept(fd)) > 0) {
			printf("New client (%d)!\n", nbClients);
			nbClients++;
			clients[nbClients].fd = newFd;
		}

		// Read requests from clients
		for (client = 1; client <= nbClients; client++) {
			if ((lRead = tsock_read(clients[client].fd, buf, sizeof(buf))) > 0) {
				handleClientRequest(client, buf, lRead);
			}
		}
	}

	tsock_close(fd);
}
