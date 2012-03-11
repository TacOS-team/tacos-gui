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
      this->id = id;
      this->parent = parent;
      this->x = x;
      this->y = y;
      this->width = width;
      this->height = height;
    }

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
struct RqClearWindow : public PronRequest {
  /** Constructor. */
  RqClearWindow(int window)
    : PronRequest(RQ_CLEAR_WINDOW) {
      this->window = window;
    }

  int window; /**< id of the window to clear */
};

/**
 * MapWindow request.
 * Sent by a client to show a window on the screen.
 */
struct RqMapWindow : public PronRequest {
  /** Constructor. */
  RqMapWindow(int window)
    : PronRequest(RQ_MAP_WINDOW) {
      this->window = window;
    }

  int window; /**< id of the window to map */
};

/**
 * RaiseWindow request.
 * Sent by a client to raise a window on the screen.
 */
struct RqRaiseWindow : public PronRequest {
  /** Constructor. */
  RqRaiseWindow(int window)
    : PronRequest(RQ_RAISE_WINDOW) {
      this->window = window;
    }

  int window; /**< id of the window to raise */
};

/**
 * CreateGC request.
 * Sent by a client to create a new graphics context.
 */
struct RqCreateGC : public PronRequest {
  /** Constructor. */
  RqCreateGC(int gc)
    : PronRequest(RQ_CREATE_GC){
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
 * DrawRect request.
 * Sent by a client to draw a rectangle to (x,y) width height sized.
 */
struct RqDrawRect : public PronRequest {
  /** Constructor. */
  RqDrawRect(int gc, int drawable, int x, int y, int width, int height)
    : PronRequest(RQ_DRAW_RECT) {
      this->gc = gc;
      this->drawable = drawable;
      this->x = x;
      this->y = y;
      this->width = width;
      this->height = height;
    }

  MessageType type; /**< message type, always first */
  int gc; /**< id of the graphics context to use */
  int drawable; /**< id of the drawable in which to draw */ 
  int x; /**< x-coordinate of the point to join */
  int y; /**< y-coordinate of the point to join */
  int width; /**< width of the rectangle */
  int height; /**< height of the rectangle */
};


/**
 * FillRectangle request.
 * Sent by a client to fill a rectangle to (x,y) width height sized.
 */
struct RqFillRectangle : public PronRequest {
  /** Constructor. */
  RqFillRectangle(int gc, int drawable, int x, int y, int width, int height)
    : PronRequest(RQ_FILL_RECTANGLE) {
      this->gc = gc;
      this->drawable = drawable;
      this->x = x;
      this->y = y;
      this->width = width;
      this->height = height;
    }

  MessageType type; /**< message type, always first */
  int gc; /**< id of the graphics context to use */
  int drawable; /**< id of the drawable in which to draw */ 
  int x; /**< x-coordinate of the point to join */
  int y; /**< y-coordinate of the point to join */
  int width; /**< width of the rectangle */
  int height; /**< height of the rectangle */
};

/**
 * DrawCircle request.
 * Sent by a client to draw a circle with given center (x,y) and radius
 */
struct RqDrawCircle : public PronRequest {
  /** Constructor. */
  RqDrawCircle(int gc, int drawable, int x, int y, int radius)
    : PronRequest(RQ_DRAW_CIRCLE) {
      this->gc = gc;
      this->drawable = drawable;
      this->x = x;
      this->y = y;
      this->radius = radius;
    }

  MessageType type; /**< message type, always first */
  int gc; /**< id of the graphics context to use */
  int drawable; /**< id of the drawable in which to draw */ 
  int x; /**< x-coordinate of the point to join */
  int y; /**< y-coordinate of the point to join */
  int radius; /**< radius of the circle */
};

/**
 * FillCircle request.
 * Sent by a client to draw a filled circle with given center (x,y) and radius
 */
struct RqFillCircle : public PronRequest {
  /** Constructor. */
  RqFillCircle(int gc, int drawable, int x, int y, int radius)
    : PronRequest(RQ_FILL_CIRCLE) {
      this->gc = gc;
      this->drawable = drawable;
      this->x = x;
      this->y = y;
      this->radius = radius;
    }

  MessageType type; /**< message type, always first */
  int gc; /**< id of the graphics context to use */
  int drawable; /**< id of the drawable in which to draw */ 
  int x; /**< x-coordinate of the point to join */
  int y; /**< y-coordinate of the point to join */
  int radius; /**< radius of the circle */
};

/**
 * GetWindowAttributes request.
 * Sent by a client to get the attributes of a window
 */
struct RqGetWindowAttributes : public PronRequest {
  /** Constructor. */
  RqGetWindowAttributes(unsigned int w)
    : PronRequest(RQ_GET_WINDOW_ATTRIBUTES) {
      this->w = w;
    }

  unsigned int w; /**< id of Window */
};

/**
 * SetWindowAttributes request.
 * Sent by a client to set the attributes of a window
 */
struct RqSetWindowAttributes : public PronRequest {
  /** Constructor. */
  RqSetWindowAttributes(unsigned int w , PronWindowAttributes newAttr,unsigned int mask)
    : PronRequest(RQ_SET_WINDOW_ATTRIBUTES) {
      this->w = w;
      this->newAttr = newAttr;
      this->mask = mask;
    }

  unsigned int w; /**< id of Window */
  PronWindowAttributes newAttr; /**< new attributes of the window */
  unsigned int mask; /**< indiquates which attributes have to be set */
};


/**
 * SelectInput request.
 * Sent by a client to subscribe to events.
 */
struct RqSelectInput : public PronRequest {
  /** Constructor. */
  RqSelectInput(unsigned int window, unsigned int eventMask)
    : PronRequest(RQ_SELECT_INPUT) {
      this->window = window;
      this->eventMask = eventMask;
    }

  unsigned int window; /**< id of Window */
  unsigned int eventMask; /**< event mask to set */
};


/**
 * Reparent request.
 * Sent by a client to set a new parent to a window.
 */
struct RqReparent : public PronRequest {
  /** Constructor. */
  RqReparent(unsigned int window, unsigned int newParent)
    : PronRequest(RQ_REPARENT) {
      this->window    = window;
      this->newParent = newParent;
    }

  unsigned int window; /**< id of Window */
  unsigned int newParent; /**< id of new parent Window */
};

#endif // __PRONLIB_REQUESTS_H__
