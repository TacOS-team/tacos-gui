#ifndef __PRONLIB_REQUESTS_H__
#define __PRONLIB_REQUESTS_H__

#include <pron_messages.h>


struct PronRequest : public PronMessage {
    /** Constructor. */
    PronRequest(MessageType type)
	: PronMessage(type) {
	}
};

/**
 * Hello request.
 * Sent by a client as the first message of the handshake.
 */
struct RqHello : public PronRequest {
    /** Constructor. */
    RqHello(int protoVersion)
	: PronRequest(RQ_HELLO) {
	    this->protoVersion = protoVersion;
	}

    int protoVersion; /**< protocol version used */
};




/**
 * CreateWindow request.
 * Sent by a client to create a new window.
 */
struct RqCreateWindow : public PronRequest {
    /** Constructor. */
    RqCreateWindow(int id, int parent, int x, int y, int width, int height)
	: PronRequest(RQ_CREATE_WINDOW) {
	    this->type = RQ_CREATE_WINDOW;
	    this->id = id;
	    this->parent = parent;
	    this->x = x;
	    this->y = y;
	    this->width = width;
	    this->height = height;
	}

    int id; /**< id of the window to create */
    unsigned int parent; /**< id of the parent window */
    int x; /**< y-coordinate of the left-top corner of the window */
    int y; /**< y-coordinate of the left-top corner of the window */
    int width; /**< width of the window */
    int height; /**< height of the window */
};

/**
 * ClearWindow request.
 * Sent by a client to clear the contents of a window.
 */
struct RqClearWindow : public PronRequest {
    /** Constructor. */
    RqClearWindow(int window)
	: PronRequest(RQ_CLEAR_WINDOW) {
	    this->type = RQ_CLEAR_WINDOW;
	    this->window = window;
	}

    unsigned int window; /**< id of the window to clear */
};

/**
 * MapWindow request.
 * Sent by a client to show a window on the screen.
 */
struct RqMapWindow : public PronRequest {
    /** Constructor. */
    RqMapWindow(int window)
	: PronRequest(RQ_MAP_WINDOW) {
	    this->type = RQ_MAP_WINDOW;
	    this->window = window;
	}

    unsigned int window; /**< id of the window to map */
};

/**
 * CreateGC request.
 * Sent by a client to create a new graphics context.
 */
struct RqCreateGC : public PronRequest {
    /** Constructor. */
    RqCreateGC(int gc)
	: PronRequest(RQ_CREATE_GC){
	    this->type = RQ_CREATE_GC;
	    this->id = id;
	}

    int id; /**< id of the graphics context to create */
};

/**
 * DrawLine request.
 * Sent by a client to draw a line between two points.
 */
struct RqDrawLine : public PronRequest {
    /** Constructor. */
    RqDrawLine(int gc, int drawable, int x1, int y1, int x2, int y2)
	: PronRequest(RQ_DRAW_LINE) {
	    this->type = RQ_DRAW_LINE;
	    this->gc = gc;
	    this->drawable = drawable;
	    this->x1 = x1;
	    this->y1 = y1;
	    this->x2 = x2;
	    this->y2 = y2;
	}

    int gc; /**< id of the graphics context to use */
    int drawable; /**< id of the drawable in which to draw */ 
    int x1; /**< x-coordinate of the first point to join */
    int y1; /**< y-coordinate of the first point to join */
    int x2; /**< x-coordinate of the second point to join */
    int y2; /**< y-coordinate of the second point to join */
};


/**
 * FillRectangle request.
 * Sent by a client to draw the rectangle (x,y) .
 */
struct RqFillRectangle : public PronRequest {
    /** Constructor. */
    RqFillRectangle(int gc, int drawable, int x1, int y1, int x2, int y2)
	: PronRequest(RQ_FILL_RECTANGLE) {
	    this->type = RQ_FILL_RECTANGLE;
	    this->gc = gc;
	    this->drawable = drawable;
	    this->x1 = x1;
	    this->y1 = y1;
	    this->x2 = x2;
	    this->y2 = y2;
	}

    int gc; /**< id of the graphics context to use */
    int drawable; /**< id of the drawable in which to draw */ 
    int x1; /**< x-coordinate of the first point to join */
    int y1; /**< y-coordinate of the first point to join */
    int x2; /**< x-coordinate of the second point to join */
    int y2; /**< y-coordinate of the second point to join */
};


/**
 * GetWindowAttributes request.
 * Sent by a client to get the attributes of a window
 */
struct RqGetWindowAttributes : public PronRequest {
    /** Constructor. */
    RqGetWindowAttributes(unsigned int w)
	: PronRequest(RQ_GET_WINDOW_ATTRIBUTES) {
	    this->type = RQ_GET_WINDOW_ATTRIBUTES;
	    this->w = w;
	}

    unsigned int w; /**< id of Window */
};


#endif // __PRONLIB_REQUESTS_H__
