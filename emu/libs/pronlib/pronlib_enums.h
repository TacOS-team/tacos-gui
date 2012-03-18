#ifndef __PRONLIB_ENUMS_H__
#define __PRONLIB_ENUMS_H__

#define RQ_PREFIX (0 << 16)
#define RS_PREFIX (1 << 16)
#define EV_PREFIX (2 << 16)
#define ER_PREFIX (3 << 16)
#define CATEGORY_MASK (3 << 16)

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
  RQ_REPARENT                 = (RQ_PREFIX | 15),
  RQ_UNMAP_WINDOW             = (RQ_PREFIX | 16),
  RQ_DRAW_POINT               = (RQ_PREFIX | 17),
  RQ_DESTROY_WINDOW           = (RQ_PREFIX | 18),
  RQ_MOVE_WINDOW              = (RQ_PREFIX | 19),
  RQ_DONT_PROPAGATE           = (RQ_PREFIX | 20),
  RQ_GET_GC_VALUES            = (RQ_PREFIX | 21),
  RQ_CHANGE_GC                = (RQ_PREFIX | 22),
  RQ_FREE_GC                  = (RQ_PREFIX | 23),
  RQ_RESIZE_WINDOW            = (RQ_PREFIX | 24),
  /* Responses */
  RS_WELCOME                  = (RS_PREFIX |  1),
  RS_WINDOW_ATTRIBUTES        = (RS_PREFIX |  2),
  RS_GC_VALUES                = (RS_PREFIX |  3),
  /* Events */
  EV_WINDOW_CREATED           = (EV_PREFIX |  1),
  EV_POINTER_MOVED            = (EV_PREFIX |  2),
  EV_EXPOSE                   = (EV_PREFIX |  3),
  EV_KEY_PRESSED              = (EV_PREFIX |  4),
  EV_KEY_RELEASED             = (EV_PREFIX |  5),
  EV_MOUSE_BUTTON             = (EV_PREFIX |  6),
  EV_DESTROY_WINDOW           = (EV_PREFIX |  7),
  EV_RESIZE_WINDOW            = (EV_PREFIX |  8),

  /* Errors */
};

#define PRON_EVENTMASK(n) (1 << (n & ~CATEGORY_MASK))

/** Pron window attributes fields for mask on get/set attributes */
enum WindowAttributeField {
  WIN_ATTR_X        = (1 << 0), 
  WIN_ATTR_Y        = (1 << 1), 
  WIN_ATTR_WIDTH    = (1 << 2), 
  WIN_ATTR_HEIGHT   = (1 << 3),
  WIN_ATTR_BG_COLOR = (1 << 4),
};

/** Pron GC values fields for mask on get/set values */
enum GCValueField {
  GC_VAL_FG        = (1 << 0), 
  GC_VAL_BG        = (1 << 1),
};


#define PRON_RELEASED    0
#define PRON_PRESSED     1

/**
 * Taken from the SDL
 */
enum PronKey {
  /*
   * ASCII mapped keysyms
   * The keyboard syms have been cleverly chosen to map to ASCII
   */
  PRONK_UNKNOWN        = 0,
  PRONK_FIRST          = 0,
  PRONK_BACKSPACE      = 8,
  PRONK_TAB            = 9,
  PRONK_CLEAR          = 12,
  PRONK_RETURN         = 13,
  PRONK_PAUSE          = 19,
  PRONK_ESCAPE         = 27,
  PRONK_SPACE          = 32,
  PRONK_EXCLAIM        = 33,
  PRONK_QUOTEDBL       = 34,
  PRONK_HASH           = 35,
  PRONK_DOLLAR         = 36,
  PRONK_AMPERSAND      = 38,
  PRONK_QUOTE          = 39,
  PRONK_LEFTPAREN      = 40,
  PRONK_RIGHTPAREN     = 41,
  PRONK_ASTERISK       = 42,
  PRONK_PLUS           = 43,
  PRONK_COMMA          = 44,
  PRONK_MINUS          = 45,
  PRONK_PERIOD         = 46,
  PRONK_SLASH          = 47,
  PRONK_0              = 48,
  PRONK_1              = 49,
  PRONK_2              = 50,
  PRONK_3              = 51,
  PRONK_4              = 52,
  PRONK_5              = 53,
  PRONK_6              = 54,
  PRONK_7              = 55,
  PRONK_8              = 56,
  PRONK_9              = 57,
  PRONK_COLON          = 58,
  PRONK_SEMICOLON      = 59,
  PRONK_LESS           = 60,
  PRONK_EQUALS         = 61,
  PRONK_GREATER        = 62,
  PRONK_QUESTION       = 63,
  PRONK_AT             = 64,
  /* Skip uppercase letters */
  PRONK_LEFTBRACKET    = 91,
  PRONK_BACKSLASH      = 92,
  PRONK_RIGHTBRACKET   = 93,
  PRONK_CARET          = 94,
  PRONK_UNDERSCORE     = 95,
  PRONK_BACKQUOTE      = 96,
  PRONK_a              = 97,
  PRONK_b              = 98,
  PRONK_c              = 99,
  PRONK_d              = 100,
  PRONK_e              = 101,
  PRONK_f              = 102,
  PRONK_g              = 103,
  PRONK_h              = 104,
  PRONK_i              = 105,
  PRONK_j              = 106,
  PRONK_k              = 107,
  PRONK_l              = 108,
  PRONK_m              = 109,
  PRONK_n              = 110,
  PRONK_o              = 111,
  PRONK_p              = 112,
  PRONK_q              = 113,
  PRONK_r              = 114,
  PRONK_s              = 115,
  PRONK_t              = 116,
  PRONK_u              = 117,
  PRONK_v              = 118,
  PRONK_w              = 119,
  PRONK_x              = 120,
  PRONK_y              = 121,
  PRONK_z              = 122,
  PRONK_DELETE         = 127,
  /* End of ASCII mapped keysyms */

  /* International keyboard syms */
  PRONK_WORLD_0        = 160,  /* 0xA0 */
  PRONK_WORLD_1        = 161,
  PRONK_WORLD_2        = 162,
  PRONK_WORLD_3        = 163,
  PRONK_WORLD_4        = 164,
  PRONK_WORLD_5        = 165,
  PRONK_WORLD_6        = 166,
  PRONK_WORLD_7        = 167,
  PRONK_WORLD_8        = 168,
  PRONK_WORLD_9        = 169,
  PRONK_WORLD_10       = 170,
  PRONK_WORLD_11       = 171,
  PRONK_WORLD_12       = 172,
  PRONK_WORLD_13       = 173,
  PRONK_WORLD_14       = 174,
  PRONK_WORLD_15       = 175,
  PRONK_WORLD_16       = 176,
  PRONK_WORLD_17       = 177,
  PRONK_WORLD_18       = 178,
  PRONK_WORLD_19       = 179,
  PRONK_WORLD_20       = 180,
  PRONK_WORLD_21       = 181,
  PRONK_WORLD_22       = 182,
  PRONK_WORLD_23       = 183,
  PRONK_WORLD_24       = 184,
  PRONK_WORLD_25       = 185,
  PRONK_WORLD_26       = 186,
  PRONK_WORLD_27       = 187,
  PRONK_WORLD_28       = 188,
  PRONK_WORLD_29       = 189,
  PRONK_WORLD_30       = 190,
  PRONK_WORLD_31       = 191,
  PRONK_WORLD_32       = 192,
  PRONK_WORLD_33       = 193,
  PRONK_WORLD_34       = 194,
  PRONK_WORLD_35       = 195,
  PRONK_WORLD_36       = 196,
  PRONK_WORLD_37       = 197,
  PRONK_WORLD_38       = 198,
  PRONK_WORLD_39       = 199,
  PRONK_WORLD_40       = 200,
  PRONK_WORLD_41       = 201,
  PRONK_WORLD_42       = 202,
  PRONK_WORLD_43       = 203,
  PRONK_WORLD_44       = 204,
  PRONK_WORLD_45       = 205,
  PRONK_WORLD_46       = 206,
  PRONK_WORLD_47       = 207,
  PRONK_WORLD_48       = 208,
  PRONK_WORLD_49       = 209,
  PRONK_WORLD_50       = 210,
  PRONK_WORLD_51       = 211,
  PRONK_WORLD_52       = 212,
  PRONK_WORLD_53       = 213,
  PRONK_WORLD_54       = 214,
  PRONK_WORLD_55       = 215,
  PRONK_WORLD_56       = 216,
  PRONK_WORLD_57       = 217,
  PRONK_WORLD_58       = 218,
  PRONK_WORLD_59       = 219,
  PRONK_WORLD_60       = 220,
  PRONK_WORLD_61       = 221,
  PRONK_WORLD_62       = 222,
  PRONK_WORLD_63       = 223,
  PRONK_WORLD_64       = 224,
  PRONK_WORLD_65       = 225,
  PRONK_WORLD_66       = 226,
  PRONK_WORLD_67       = 227,
  PRONK_WORLD_68       = 228,
  PRONK_WORLD_69       = 229,
  PRONK_WORLD_70       = 230,
  PRONK_WORLD_71       = 231,
  PRONK_WORLD_72       = 232,
  PRONK_WORLD_73       = 233,
  PRONK_WORLD_74       = 234,
  PRONK_WORLD_75       = 235,
  PRONK_WORLD_76       = 236,
  PRONK_WORLD_77       = 237,
  PRONK_WORLD_78       = 238,
  PRONK_WORLD_79       = 239,
  PRONK_WORLD_80       = 240,
  PRONK_WORLD_81       = 241,
  PRONK_WORLD_82       = 242,
  PRONK_WORLD_83       = 243,
  PRONK_WORLD_84       = 244,
  PRONK_WORLD_85       = 245,
  PRONK_WORLD_86       = 246,
  PRONK_WORLD_87       = 247,
  PRONK_WORLD_88       = 248,
  PRONK_WORLD_89       = 249,
  PRONK_WORLD_90       = 250,
  PRONK_WORLD_91       = 251,
  PRONK_WORLD_92       = 252,
  PRONK_WORLD_93       = 253,
  PRONK_WORLD_94       = 254,
  PRONK_WORLD_95       = 255,  /* 0xFF */

  /* Numeric keypad */
  PRONK_KP0            = 256,
  PRONK_KP1            = 257,
  PRONK_KP2            = 258,
  PRONK_KP3            = 259,
  PRONK_KP4            = 260,
  PRONK_KP5            = 261,
  PRONK_KP6            = 262,
  PRONK_KP7            = 263,
  PRONK_KP8            = 264,
  PRONK_KP9            = 265,
  PRONK_KP_PERIOD      = 266,
  PRONK_KP_DIVIDE      = 267,
  PRONK_KP_MULTIPLY    = 268,
  PRONK_KP_MINUS       = 269,
  PRONK_KP_PLUS        = 270,
  PRONK_KP_ENTER       = 271,
  PRONK_KP_EQUALS      = 272,

  /* Arrows + Home/End pad */
  PRONK_UP             = 273,
  PRONK_DOWN           = 274,
  PRONK_RIGHT          = 275,
  PRONK_LEFT           = 276,
  PRONK_INSERT         = 277,
  PRONK_HOME           = 278,
  PRONK_END            = 279,
  PRONK_PAGEUP         = 280,
  PRONK_PAGEDOWN       = 281,

  /* Function keys */
  PRONK_F1             = 282,
  PRONK_F2             = 283,
  PRONK_F3             = 284,
  PRONK_F4             = 285,
  PRONK_F5             = 286,
  PRONK_F6             = 287,
  PRONK_F7             = 288,
  PRONK_F8             = 289,
  PRONK_F9             = 290,
  PRONK_F10            = 291,
  PRONK_F11            = 292,
  PRONK_F12            = 293,
  PRONK_F13            = 294,
  PRONK_F14            = 295,
  PRONK_F15            = 296,

  /* Key state modifier keys */
  PRONK_NUMLOCK        = 300,
  PRONK_CAPSLOCK       = 301,
  PRONK_SCROLLOCK      = 302,
  PRONK_RSHIFT         = 303,
  PRONK_LSHIFT         = 304,
  PRONK_RCTRL          = 305,
  PRONK_LCTRL          = 306,
  PRONK_RALT           = 307,
  PRONK_LALT           = 308,
  PRONK_RMETA          = 309,
  PRONK_LMETA          = 310,
  PRONK_LSUPER         = 311,  /* Left "Windows" key */
  PRONK_RSUPER         = 312,  /* Right "Windows" key */
  PRONK_MODE           = 313,  /* "Alt Gr" key */
  PRONK_COMPOSE        = 314,  /* Multi-key compose key */

  /* Miscellaneous function keys */
  PRONK_HELP           = 315,
  PRONK_PRINT          = 316,
  PRONK_SYSREQ         = 317,
  PRONK_BREAK          = 318,
  PRONK_MENU           = 319,
  PRONK_POWER          = 320,  /* Power Macintosh power key */
  PRONK_EURO           = 321,  /* Some european keyboards */
  PRONK_UNDO           = 322,  /* Atari keyboard has Undo */

  /* Add any other keys here */
  PRONK_LAST
};

/** Enumeration of valid key mods (possibly OR'd together) */
enum PronMod {
  KMOD_NONE      = 0x0000,
  KMOD_LSHIFT    = 0x0001,
  KMOD_RSHIFT    = 0x0002,
  KMOD_LCTRL     = 0x0040,
  KMOD_RCTRL     = 0x0080,
  KMOD_LALT      = 0x0100,
  KMOD_RALT      = 0x0200,
  KMOD_LMETA     = 0x0400,
  KMOD_RMETA     = 0x0800,
  KMOD_NUM       = 0x1000,
  KMOD_CAPS      = 0x2000,
  KMOD_MODE      = 0x4000,
  KMOD_RESERVED  = 0x8000
};

#define KMOD_CTRL   (KMOD_LCTRL | KMOD_RCTRL)
#define KMOD_SHIFT  (KMOD_LSHIFT  | KMOD_RSHIFT)
#define KMOD_ALT    (KMOD_LALT | KMOD_RALT)
#define KMOD_META   (KMOD_LMETA | KMOD_RMETA)

/**
 * Image pixmap types
 */
enum PixmapType{
  XYBitmap = 0x0000, 
  XYPixmap = 0x0001, 
  ZPixmap  = 0X0002
};

#endif // __PRONLIB_ENUMS_H__
