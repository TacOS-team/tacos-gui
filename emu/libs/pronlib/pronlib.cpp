#include <pronlib.hpp>
#include <pron_types.hpp>
#include <tsock.h>
#include <cstdio>

Display* pronConnect() {
	Display *d = new Display();
	int fd = tsock_connect("/tmp/pron.sock");
	if (fd < -1) {
		fprintf(stderr, "FUQUE.\n");
	}

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

void pronSubscribeEvent(Display *d, uint32 events) {
	
}

