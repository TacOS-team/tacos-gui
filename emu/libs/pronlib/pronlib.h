#ifndef _PRONLIB_H_
#define _PRONLIB_H_

#include <pron_proto.h>
#include <stdint.h>

typedef int Window;
typedef int GC;

struct Display {
	Display(int fd, RespWelcome *welcome) {
		this->fd = fd;
		this->rootWindow = welcome->rootWindow;
		this->startId = welcome->startId;
		this->endId = welcome->endId;
		this->curId = this->startId;
	}

	int newResourceId() {
		return this->curId++;
	}

	int fd;
	Window rootWindow;
	int startId;
	int endId;
	int curId;
};

Display* pronConnect();

Window pronCreateWindow(Display *d, Window parent, int x, int y, int width, int height);

void pronClearWindow(Display *d, Window w);

void pronFlushWindow(Display *d, Window w);

GC pronCreateGC(Display *d);

void pronMapWindow(Display *d, Window w);

void pronDrawLine(Display *d, Window w, GC gc, int x1, int y1, int x2, int y2);

void pronDisconnect(Display *d);

void pronSubscribeEvent(Display *d, uint32_t events);

#endif
