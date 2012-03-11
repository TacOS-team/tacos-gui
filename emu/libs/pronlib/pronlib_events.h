#ifndef __PRONLIB_EVENTS_H__
#define __PRONLIB_EVENTS_H__

#include <pron_messages.h>
#include <stdlib.h>

struct PronEvent : public PronMessage {
  /** Constructor. */
  PronEvent(MessageType type)
      : PronMessage(type) {}
};

/**
 * ClearWindow request.
 * Sent by a client to clear the contents of a window.
 */
struct EventWindowCreated : public PronEvent {
  /** Constructor. */
  EventWindowCreated(unsigned int window, unsigned int parent, PronWindowAttributes attributes)
       : PronEvent(EV_WINDOW_CREATED) {
     this->window = window;
     this->parent = parent;
     this->attributes = attributes;
  }

  unsigned int window;
  unsigned int parent;
  PronWindowAttributes attributes;
};

/**
 * EventExpose
 */
struct EventExpose : public PronEvent {
  /** Constructor. */
  EventExpose(unsigned int window, int x, int y, int width, int height)
       : PronEvent(EV_EXPOSE) {
     this->window = window;
     this->x = x;
     this->y = y;
     this->width = width;
     this->height = height;
  }

  unsigned int window;
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
       : PronEvent(EV_POINTER_MOVED) {
    this->window = window;
    this->x = x;
    this->y = y;
    this->xRoot = xRoot;
    this->yRoot = yRoot;
  }

  unsigned int window;
  int x, y;
  int xRoot, yRoot;
};

/**
 * EventKey
 */
struct EventKey : public PronEvent {
  /** Constructor. */
  EventKey(MessageType type, unsigned int window, unsigned int subwindow,
        int x, int y, int xRoot, int yRoot, unsigned int keysym, unsigned int modifiers)
        : PronEvent(type) {
    this->window = window;
    this->subwindow = subwindow;
    this->x = x;
    this->y = y;
    this->xRoot = xRoot;
    this->yRoot = yRoot;
    this->keysym = keysym;
    this->modifiers = modifiers;
  }

  unsigned int window;
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

PronEvent* getPronEvent();

#endif // __PRONLIB_EVENTS_H__
