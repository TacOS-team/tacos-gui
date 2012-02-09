#include <pronlib.h>
extern "C" {
#include <tsock.h>
}
#include <cstdio>

#include <stdint.h>

Display* pronConnect() {
	Display *d = new Display();
	int fd = tsock_connect("/tmp/pron.sock");
	if (fd < -1) {
		fprintf(stderr, "FUQUE.\n");
	}
	RqHello h(42);
	tsock_write(fd, &h, sizeof(h));

	return d;	
}

Window pronCreateWindow(Display *d, Window parent, int x, int y, int width, int height) {
	Window w;
	return w;
}

GC pronCreateGC(Display *d) {
	GC gc;
	return gc;
}

void pronMapWindow(Display *d, Window w) {

}

void pronDrawLine(Display *d, Window w, GC gc, int x1, int y1, int x2, int y2) {

}

void pronDisconnect(Display *d) {
	
}

void pronSubscribeEvent(Display *d, uint32_t events) {
	
}

