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
  EventWindowCreated(unsigned int window, PronWindowAttributes attributes)
       : PronEvent(EVENT_WINDOW_CREATED) {
     this->window = window;
     this->attributes = attributes;
  }

  unsigned int window;
  PronWindowAttributes attributes;
};

/**
 * EventPointerMoved
 */
struct EventPointerMoved : public PronEvent {
  /** Constructor. */
  EventPointerMoved(unsigned int window, PronWindowAttributes attributes)
       : PronEvent(EVENT_POINTER_MOVED) {
  }

  int x, y;		/* pointer x, y coordinates in event window */
  int x_root, y_root;	/* coordinates relative to root */
};

PronEvent * getPronEvent() {
  int size = sizeof(EventWindowCreated);
  
  return (PronEvent *) malloc(size);
}


#endif // __PRONLIB_EVENTS_H__

