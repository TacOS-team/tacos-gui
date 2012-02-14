#ifndef __PRONLIB_ENUMS_H__
#define __PRONLIB_ENUMS_H__

/** Hack to have the MessageType as string */
#define MESSAGE_TYPE                          \
ENUM_OR_STRING( RQ_HELLO ),                   \
ENUM_OR_STRING( RQ_CREATE_WINDOW ),           \
ENUM_OR_STRING( RQ_CLEAR_WINDOW ),            \
ENUM_OR_STRING( RQ_MAP_WINDOW ),              \
ENUM_OR_STRING( RQ_CREATE_GC ),               \
ENUM_OR_STRING( RQ_DRAW_LINE ),               \
ENUM_OR_STRING( RQ_FILL_RECTANGLE ),          \
ENUM_OR_STRING( RQ_GET_WINDOW_ATTRIBUTES ),   \
ENUM_OR_STRING( RQ_SELECT_INPUT ),            \
ENUM_OR_STRING( RESP_WELCOME ),               \
ENUM_OR_STRING( RESP_WINDOW_ATTRIBUTES )

#undef ENUM_OR_STRING
#define ENUM_OR_STRING( x ) x

/** Pron message types. */
enum MessageType {
  MESSAGE_TYPE
};

#ifdef DEBUG

#undef ENUM_OR_STRING
#define ENUM_OR_STRING( x ) #x

__attribute__((unused))
static const char * MessageTypeStrings[] = {
  MESSAGE_TYPE
};

#endif// DEBUG

#undef ENUM_OR_STRING

/** Pron event types. */
enum EventType {
  CREATE_NOTIFY = 1 << 0,
};

/** Pron window attributes fields for mask on get/set attributes */
enum WindowAttributeField {
  WIN_ATTR_X = (1 << 0), 
  WIN_ATTR_Y = (1 << 1), 
  WIN_ATTR_WIDTH = (1 << 2), 
  WIN_ATTR_HEIGHT = (1 << 3)
};  

#endif // __PRONLIB_ENUMS_H__

