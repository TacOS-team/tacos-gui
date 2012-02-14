#ifndef __PRONLIB_EVENTS_H__
#define __PRONLIB_EVENTS_H__


#include <pron_messages.h>
#include <stdlib.h>


struct PronEvent : public PronMessage {
	/** Constructor. */
	PronEvent(MessageType type)
	    : PronMessage(type) {
	}
	
};


/**
 * ClearWindow request.
 * Sent by a client to clear the contents of a window.
 */
struct EventWindowCreated : public PronEvent {
  /** Constructor. */
  EventWindowCreated()
       : PronEvent(EVENT_WINDOW_CREATED) {
  }
  EventWindowCreated(PronWindowAttributes attributes)
       : PronEvent(EVENT_WINDOW_CREATED) {
     this->attributes = attributes;
  }

  PronWindowAttributes attributes;
};


PronEvent * getPronEvent() {
  int size = sizeof(EventWindowCreated);
  
  return (PronEvent *) malloc(size);
}


#endif // __PRONLIB_EVENTS_H__

