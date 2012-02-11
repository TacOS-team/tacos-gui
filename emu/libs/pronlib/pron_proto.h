#ifndef _PRON_PROTO_H_
#define _PRON_PROTO_H_

enum MessageType {
	RQ_HELLO,
	RESP_WELCOME,
	RQ_CREATE_WINDOW,
	RQ_CLEAR_WINDOW,
	RQ_FLUSH_WINDOW,
	RQ_MAP_WINDOW,
	RQ_CREATE_GC,
	RQ_DRAW_LINE
};

enum EventType {
	CREATE_NOTIFY = 1 << 0,
};

struct RqHello {
	RqHello(int protoVersion) {
		this->type = RQ_HELLO;
		this->protoVersion = protoVersion;
	}

	int type;
	int protoVersion;
};

struct RespWelcome{
	RespWelcome(int rootWindow, int startId, int endId) {
		this->type = RESP_WELCOME;
		this->rootWindow = rootWindow;
		this->startId = startId;
		this->endId = endId;
	}

	int type;
	int rootWindow;
	int startId;
	int endId;
};

struct RqCreateWindow {
	RqCreateWindow(int id, int parent, int x, int y, int width, int height) {
		this->type = RQ_CREATE_WINDOW;
		this->id = id;
		this->parent = parent;
		this->x = x;
		this->y = y;
		this->width = width;
		this->height = height;
	}

	int type;
	int id;
	int parent;
	int x, y;
	int width, height;	
};

struct RqClearWindow {
	RqClearWindow(int window) {
		this->type = RQ_CLEAR_WINDOW;
		this->window = window;
	}

	int type;
	int window;
};

struct RqFlushWindow {
	RqFlushWindow(int window) {
		this->type = RQ_FLUSH_WINDOW;
		this->window = window;
	}

	int type;
	int window;
};

struct RqMapWindow {
	RqMapWindow(int window) {
		this->type = RQ_MAP_WINDOW;
		this->window = window;
	}

	int type;
	int window;
};

struct RqCreateGC {
	RqCreateGC(int gc) {
		this->type = RQ_CREATE_GC;
		this->id = id;
	}

	int type;
	int id;
};

struct RqDrawLine {
	RqDrawLine(int gc, int drawable, int x1, int y1, int x2, int y2) {
		this->type = RQ_DRAW_LINE;
		this->gc = gc;
		this->drawable = drawable;
		this->x1 = x1;
		this->y1 = y1;
		this->x2 = x2;
		this->y2 = y2;
	}

	int type;
	int gc;
	int drawable;
	int x1, y1;
	int x2, y2;
};

#endif
