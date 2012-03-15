#ifndef __PRONLIB_ERRORS_H__
#define __PRONLIB_ERRORS_H__

#include <pron_messages.h>

struct PronError : public PronMessage {
  /** Constructor. */
  PronError(MessageType type)
      : PronMessage(type) {
  }
};

#endif // __PRONLIB_ERRORS_H__
