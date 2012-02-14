#ifndef __PRONLIB_EVENTS_H__
#define __PRONLIB_EVENTS_H__


#include <pron_messages.h>


struct PronEvents : public PronMessage {
	/** Constructor. */
	PronEvents(MessageType type)
	    : PronMessage(type) {
	}
};

#endif // __PRONLIB_EVENTS_H__
