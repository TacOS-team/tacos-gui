#ifndef __PRONLIB_ENUMS_H__
#define __PRONLIB_ENUMS_H__

#define RQ_PREFIX (0 << 14)
#define RS_PREFIX (1 << 14)
#define EV_PREFIX (2 << 14)
#define ER_PREFIX (3 << 14)
#define CATEGORY_MASK (3 << 14)

/** Pron message types. */
enum MessageType {
  /* Requests */
  RQ_HELLO                    = (RQ_PREFIX |  1),
  RQ_CREATE_WINDOW            = (RQ_PREFIX |  2),
  RQ_CLEAR_WINDOW             = (RQ_PREFIX |  3),
  RQ_MAP_WINDOW               = (RQ_PREFIX |  4),
  RQ_CREATE_GC                = (RQ_PREFIX |  5),
  RQ_DRAW_LINE                = (RQ_PREFIX |  6),
  RQ_DRAW_RECT                = (RQ_PREFIX |  7),
  RQ_FILL_RECTANGLE           = (RQ_PREFIX |  8),
  RQ_DRAW_CIRCLE              = (RQ_PREFIX |  9),
  RQ_FILL_CIRCLE              = (RQ_PREFIX | 10),
  RQ_GET_WINDOW_ATTRIBUTES    = (RQ_PREFIX | 11),
  RQ_SET_WINDOW_ATTRIBUTES    = (RQ_PREFIX | 12),
  RQ_SELECT_INPUT             = (RQ_PREFIX | 13),
  RQ_RAISE_WINDOW             = (RQ_PREFIX | 14),
  /* Responses */
  RS_WELCOME                  = (RS_PREFIX |  1),
  RS_WINDOW_ATTRIBUTES        = (RS_PREFIX |  2),
  /* Events */
  EV_WINDOW_CREATED           = (EV_PREFIX |  1),
  EV_POINTER_MOVED            = (EV_PREFIX |  2),
  EV_EXPOSE                   = (EV_PREFIX |  3),
  /* Errors */
};

#define PRON_EVENTMASK(n) (1 << (n & ~CATEGORY_MASK))

/** Pron window attributes fields for mask on get/set attributes */
enum WindowAttributeField {
  WIN_ATTR_X        = (1 << 0), 
  WIN_ATTR_Y        = (1 << 1), 
  WIN_ATTR_WIDTH    = (1 << 2), 
  WIN_ATTR_HEIGHT   = (1 << 3)
};  

#endif // __PRONLIB_ENUMS_H__
