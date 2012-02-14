#ifndef __PRONLIB_MESSAGES_H__
#define __PRONLIB_MESSAGES_H__


struct PronMessage {
  /** Constructor. */
  PronMessage(MessageType type) {
    this->type = type;
  }
  MessageType type;
};

#endif // __PRONLIB_MESSAGES_H__
