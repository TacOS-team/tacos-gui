#ifndef __PRONPROTO_ATTRIBUTES_H__
#define __PRONPROTO_ATTRIBUTES_H__

#include <vesa_types.h>

namespace pron {

/** Pron window attributes fields for mask on get/set attributes */
enum WindowAttributeField {
  WIN_ATTR_X            = (1 << 0), 
  WIN_ATTR_Y            = (1 << 1), 
  WIN_ATTR_WIDTH        = (1 << 2), 
  WIN_ATTR_HEIGHT       = (1 << 3),
  WIN_ATTR_BG_COLOR     = (1 << 4),
  WIN_ATTR_IS_RESIZABLE = (1 << 5),
  WIN_ATTR_MAX_WIDTH    = (1 << 6),
  WIN_ATTR_MAX_HEIGHT   = (1 << 7),
  WIN_ATTR_MIN_WIDTH    = (1 << 8),
  WIN_ATTR_MIN_HEIGHT   = (1 << 9),
};

/**
 * Structure for pron window attributes
 */
struct PronWindowAttributes {
  int x;
  int y;
  int width;
  int height;
  color_t bgColor;
  bool isResizable;
  int maxWidth;
  int maxHeight;
  int minWidth;
  int minHeight;
};

} // namespace pron

#endif // __PRONPROTO_ATTRIBUTES_H__
