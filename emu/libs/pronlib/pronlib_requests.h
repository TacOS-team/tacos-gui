#ifndef __PRONLIB_REQUESTS_H__
#define __PRONLIB_REQUESTS_H__

/**
 * Hello request.
 * Sent by a client as the first message of the handshake.
 */
struct RqHello {
  /** Constructor. */
  RqHello(int protoVersion) {
    this->type = RQ_HELLO;
    this->protoVersion = protoVersion;
  }

  MessageType type; /**< message type, always first */
  int protoVersion; /**< protocol version used */
};


/**
 * CreateWindow request.
 * Sent by a client to create a new window.
 */
struct RqCreateWindow {
  /** Constructor. */
  RqCreateWindow(int id, int parent, int x, int y, int width, int height) {
    this->type = RQ_CREATE_WINDOW;
    this->id = id;
    this->parent = parent;
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
  }

  MessageType type; /**< message type, always first */
  int id; /**< id of the window to create */
  int parent; /**< id of the parent window */
  int x; /**< y-coordinate of the left-top corner of the window */
  int y; /**< y-coordinate of the left-top corner of the window */
  int width; /**< width of the window */
  int height; /**< height of the window */
};


/**
 * ClearWindow request.
 * Sent by a client to clear the contents of a window.
 */
struct RqClearWindow {
  /** Constructor. */
  RqClearWindow(int window) {
    this->type = RQ_CLEAR_WINDOW;
    this->window = window;
  }

  MessageType type; /**< message type, always first */
  int window; /**< id of the window to clear */
};


/**
 * MapWindow request.
 * Sent by a client to show a window on the screen.
 */
struct RqMapWindow {
  /** Constructor. */
  RqMapWindow(int window) {
    this->type = RQ_MAP_WINDOW;
    this->window = window;
  }

  MessageType type; /**< message type, always first */
  int window; /**< id of the window to map */
};


/**
 * CreateGC request.
 * Sent by a client to create a new graphics context.
 */
struct RqCreateGC {
  /** Constructor. */
  RqCreateGC(int gc) {
    this->type = RQ_CREATE_GC;
    this->id = id;
  }

  MessageType type; /**< message type, always first */
  int id; /**< id of the graphics context to create */
};


/**
 * DrawLine request.
 * Sent by a client to draw a line between two points.
 */
struct RqDrawLine {
  /** Constructor. */
  RqDrawLine(int gc, int drawable, int x1, int y1, int x2, int y2) {
    this->type = RQ_DRAW_LINE;
    this->gc = gc;
    this->drawable = drawable;
    this->x1 = x1;
    this->y1 = y1;
    this->x2 = x2;
    this->y2 = y2;
  }

  MessageType type; /**< message type, always first */
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
struct RqFillRectangle {
  /** Constructor. */
  RqFillRectangle(int gc, int drawable, int x1, int y1, int x2, int y2) {
    this->type = RQ_FILL_RECTANGLE;
    this->gc = gc;
    this->drawable = drawable;
    this->x1 = x1;
    this->y1 = y1;
    this->x2 = x2;
    this->y2 = y2;
  }

  MessageType type; /**< message type, always first */
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
struct RqGetWindowAttributes {
  /** Constructor. */
  RqGetWindowAttributes(unsigned int w) {
    this->type = RQ_GET_WINDOW_ATTRIBUTES;
    this->w = w;
  }

  MessageType type; /**< message type, always first */
  unsigned int w; /**< id of Window */
};


/**
 * SelectInput request.
 * Sent by a client to subscribe to events.
 */
struct RqSelectInput {
  /** Constructor. */
  RqSelectInput(unsigned int window, unsigned int eventMask) {
    this->type = RQ_SELECT_INPUT;
    this->window = window;
    this->eventMask = eventMask;
  }

  MessageType type; /**< message type, always first */
  unsigned int window; /**< id of Window */
  unsigned int eventMask; /**< event mask to set */
};

#endif // __PRONLIB_REQUESTS_H__
