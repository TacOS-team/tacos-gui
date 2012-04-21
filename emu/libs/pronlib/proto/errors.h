#ifndef __PRONPROTO_ERRORS_H__
#define __PRONPROTO_ERRORS_H__

#include <proto/base.h>

namespace pron {

struct PronError : public PronMessage {
  /** Constructor. */
  PronError(MessageType type)
      : PronMessage(type) {
  }
};

} // namespace pron

#endif // __PRONPROTO_ERRORS_H__
