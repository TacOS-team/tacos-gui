#ifndef __PRONPROTO_ERRORS_H__
#define __PRONPROTO_ERRORS_H__

#include <proto/base.h>

/**
 * Base class representing a pron error message.
 */
struct PronError : public PronMessage {
  /**
   * Constructor.
   * @param type The message type
   */
  PronError(MessageType type)
      : PronMessage(type) {
  }
};

#endif // __PRONPROTO_ERRORS_H__
