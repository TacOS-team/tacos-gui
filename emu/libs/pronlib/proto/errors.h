#ifndef __PRONPROTO_ERRORS_H__
#define __PRONPROTO_ERRORS_H__

#include <proto/base.h>

struct PronError : public PronMessage {
  /** Constructor. */
  PronError(MessageType type)
      : PronMessage(type) {
  }
};

#endif // __PRONPROTO_ERRORS_H__
