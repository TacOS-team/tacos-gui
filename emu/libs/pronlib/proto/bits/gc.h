#ifndef __PRONPROTO_GC_H__
#define __PRONPROTO_GC_H__

#include <vesa_types.h>

/** Pron GC values fields for mask on get/set values */
enum GCValueField {
  GC_VAL_FG        = (1 << 0), 
  GC_VAL_BG        = (1 << 1),
};

/**
 * Structure for pron graphics contexts values
 */
struct PronGCValues {
  color_t fg;
  color_t bg;
};

#endif // __PRONPROTO_GC_H__
