#ifndef __PRONPROTO_GC_H__
#define __PRONPROTO_GC_H__

#include <vesa_types.h>

/**
 * Graphics context fields masks for get/set requests.
 */
enum GCValueField {
  GC_VAL_FG        = (1 << 0), 
  GC_VAL_BG        = (1 << 1),
};

/**
 * Graphics context values.
 */
struct PronGCValues {
  color_t fg; /**< Foreground color */
  color_t bg; /**< Background color */
};

#endif // __PRONPROTO_GC_H__
