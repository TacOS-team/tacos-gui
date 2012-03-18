#ifndef __PRONLIB_RESPONSES_H__
#define __PRONLIB_RESPONSES_H__

#include <pron_messages.h>

struct PronResponse : public PronMessage {
  /** Constructor. */
  PronResponse(MessageType type)
      : PronMessage(type) {}
};

/**
 * Welcome response.
 * Sent by the server in response to a Hello request.
 */
struct RespWelcome : public PronResponse {
  /** Constructor. */
  RespWelcome(int rootWindow, int startId, int endId)
      : PronResponse(RS_WELCOME) {
    this->rootWindow = rootWindow;
    this->startId = startId;
    this->endId = endId;
  }

  int rootWindow; /**< id of the root window */
  int startId; /**< first usable resource id */
  int endId; /**< last usable resource id */
};

/**
 * WindowAttributes response.
 * Sent by the server in response to a GetWindowAttributes request.
 */
struct RespWindowAttributes : public PronResponse {
  /** Constructor. */
  RespWindowAttributes(const PronWindowAttributes & attributes)
      : PronResponse(RS_WINDOW_ATTRIBUTES){
    this->attributes = attributes;
  }
  
  PronWindowAttributes attributes; /**< Attributes of the window */
};

/**
 * GCValues response.
 * Sent by the server in response to a GetGCValues request.
 */
struct RespGCValues : public PronResponse {
  /** Constructor. */
  RespGCValues(const PronGCValues &values)
      : PronResponse(RS_GC_VALUES){
    this->values = values;
  }
  
  PronGCValues values; /**< values of the graphics context */
};

#endif // __PRONLIB_RESPONSES_H__
