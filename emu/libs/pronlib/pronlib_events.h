#ifndef __PRONLIB_EVENTS_H__
#define __PRONLIB_EVENTS_H__

#include <pron_messages.h>
#include <stdlib.h>

struct PronEvent : public PronMessage {
  /** Constructor. */
  PronEvent(MessageType type, unsigned int window)
      : PronMessage(type) {
    this->window = window;
  }

  unsigned int window;
};

/**
 * ClearWindow request.
 * Sent by a client to clear the contents of a window.
 */
struct EventWindowCreated : public PronEvent {
  /** Constructor. */
  EventWindowCreated(unsigned int window, unsigned int parent, const PronWindowAttributes &attributes)
      : PronEvent(EV_WINDOW_CREATED, window) {
    this->parent = parent;
    this->attributes = attributes;
  }

  unsigned int parent;
  PronWindowAttributes attributes;
};

/**
 * EventExpose
 */
struct EventExpose : public PronEvent {
  /** Constructor. */
  EventExpose(unsigned int window, int x, int y, int width, int height)
      : PronEvent(EV_EXPOSE, window) {
    this->x = x;
    this->y = y;
    this->width = width;
    this->height = height;
  }

  int x;
  int y;
  int width;
  int height;
};

/**
 * EventPointerMoved
 */
struct EventPointerMoved : public PronEvent {
  /** Constructor. */
  EventPointerMoved(unsigned int window, int x, int y, int xRoot, int yRoot)
      : PronEvent(EV_POINTER_MOVED, window) {
    this->x = x;
    this->y = y;
    this->xRoot = xRoot;
    this->yRoot = yRoot;
  }

  int x; 
  int y;
  int xRoot; 
  int yRoot;
};

/**
 * EventMouseButton
 */
struct EventMouseButton : public PronEvent {
  /** Constructor. */
  EventMouseButton(unsigned int window, bool b1, bool b2, bool b3, bool b4, bool b5, bool b6, 
      int x, int y, int xRoot, int yRoot)
      : PronEvent(EV_MOUSE_BUTTON, window) {
    this->b1 = b1;
    this->b2 = b2;
    this->b3 = b3;
    this->b4 = b4;
    this->b5 = b5;
    this->b6 = b6;
    this->x  = x; // Relative window coordinates
    this->y  = y;
    this->xRoot = xRoot; // Root window coordinates
    this->yRoot = yRoot;
  }

  bool b1; // left button
  bool b2;
  bool b3;
  bool b4;
  bool b5;
  bool b6;
  int  x;
  int  y;
  int xRoot
  int yRoot;
};

/**
 * EventKey
 */
struct EventKey : public PronEvent {
  /** Constructor. */
  EventKey(MessageType type, unsigned int window, unsigned int subwindow,
      int x, int y, int xRoot, int yRoot, unsigned int keysym, unsigned int modifiers)
      : PronEvent(type, window) {
    this->subwindow = subwindow;
    this->x = x;
    this->y = y;
    this->xRoot = xRoot;
    this->yRoot = yRoot;
    this->keysym = keysym;
    this->modifiers = modifiers;
  }

  unsigned int subwindow;
  int x, y;
  int xRoot, yRoot;
  unsigned int keysym;
  unsigned int modifiers;
};

/**
 * EventKeyPressed
 */
struct EventKeyPressed : public EventKey {
  /** Constructor. */
  EventKeyPressed(unsigned int window, unsigned int subwindow, int x, int y,
      int xRoot, int yRoot, unsigned int keysym, unsigned int modifiers)
      : EventKey(EV_KEY_PRESSED, window, subwindow, x, y, xRoot, yRoot, keysym, modifiers) {}
};

/**
 * EventKeyReleased
 */
struct EventKeyReleased : public EventKey {
  /** Constructor. */
  EventKeyReleased(unsigned int window, unsigned int subwindow, int x, int y,
      int xRoot, int yRoot, unsigned int keysym, unsigned int modifiers)
      : EventKey(EV_KEY_RELEASED, window, subwindow, x, y, xRoot, yRoot, keysym, modifiers) {}
};

/**
 * EventDestroyWindow
 */
struct EventDestroyWindow : public PronEvent {
  /** Constructor. */
  EventDestroyWindow(unsigned int window)
      : PronEvent(EV_DESTROY_WINDOW, window) {}
};

/**
 * EventResizeWindow asks an application to resize his window drawing
 */
struct EventResizeWindow : public PronEvent {
  /** Constructor. */
  EventResizeWindow(int width, int height)
      : PronEvent(EV_RESIZE_WINDOW, window) {
    this->width  = width;
    this->height = height;
  }
  int width;
  int height;
};

PronEvent* getPronEvent();

#endif // __PRONLIB_EVENTS_H__
