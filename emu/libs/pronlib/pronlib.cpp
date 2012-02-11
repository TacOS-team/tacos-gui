#include <pronlib.h>
extern "C" {
#include <tsock.h>
}
#include <cstdio>
#include <unistd.h>

#define MAX_MSG_SIZE 1024

Display* pronConnect() {
	int fd = tsock_connect("/tmp/pron.sock");
	if (fd < -1) {
		fprintf(stderr, "FUQUE.\n");
	}
	printf("Sending hello...\n");
	RqHello rq(42);
	tsock_write(fd, &rq, sizeof(rq));
	
	printf("Reading welcome...\n");
	int lRead;
	char buf[MAX_MSG_SIZE];
	while ((lRead = tsock_read(fd, buf, sizeof(buf))) < 0) {
		usleep(100000);
	}

	RespWelcome *resp = (RespWelcome*) buf;
	printf("Welcome from server [%x - %x, root = %d]\n", resp->startId, resp->endId, resp->rootWindow);
	Display *d = new Display(fd, resp);

	return d;	
}

Window pronCreateWindow(Display *d, Window parent, int x, int y, int width, int height) {
	Window w = d->newResourceId();
	RqCreateWindow rq(w, parent, x, y, width, height);
	tsock_write(d->fd, &rq, sizeof(rq));

	return w;
}

void pronClearWindow(Display *d, Window w) {
	RqClearWindow rq(w);
	tsock_write(d->fd, &rq, sizeof(rq));
}

void pronFlushWindow(Display *d, Window w) {
	RqFlushWindow rq(w);
	tsock_write(d->fd, &rq, sizeof(rq));
}

GC pronCreateGC(Display *d) {
	GC gc = 0;
	return gc;
}

void pronMapWindow(Display *d, Window w) {

}

void pronDrawLine(Display *d, Window w, GC gc, int x1, int y1, int x2, int y2) {
	RqDrawLine rq(gc, w, x1, y1, x2, y2);
	tsock_write(d->fd, &rq, sizeof(rq));
}

void pronDisconnect(Display *d) {
	tsock_close(d->fd);
	delete d;	
}

void pronSubscribeEvent(Display *d, uint32_t events) {
	
}

