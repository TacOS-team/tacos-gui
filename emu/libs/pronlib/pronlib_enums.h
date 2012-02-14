#ifndef __PRONLIB_ENUMS_H__
#define __PRONLIB_ENUMS_H__

/** Hack to have the MessageType as string */
#define MESSAGE_TYPE         \
  ENUM_OR_STRING( RQ_HELLO ),    \
  ENUM_OR_STRING( RESP_WELCOME ),    \
  ENUM_OR_STRING( RQ_CREATE_WINDOW ),    \
  ENUM_OR_STRING( RQ_CLEAR_WINDOW ),    \
  ENUM_OR_STRING( RQ_MAP_WINDOW ),    \
  ENUM_OR_STRING( RQ_CREATE_GC ),    \
  ENUM_OR_STRING( RQ_DRAW_LINE ),    \
  ENUM_OR_STRING( RQ_FILL_RECTANGLE ),    \
  ENUM_OR_STRING( RQ_GET_WINDOW_ATTRIBUTES ),    \
  ENUM_OR_STRING( RESP_WINDOW_ATTRIBUTES )


#undef ENUM_OR_STRING
#define ENUM_OR_STRING( x ) x

/** Pron message types. */
enum MessageType {
	MESSAGE_TYPE
};

#undef ENUM_OR_STRING
#define ENUM_OR_STRING( x ) #x

const char * MessageTypeStrings[] = {
    MESSAGE_TYPE
};

#undef ENUM_OR_STRING

/** Pron event types. */
enum EventType {
	CREATE_NOTIFY = 1 << 0,
};


#endif // __PRONLIB_ENUMS_H__

