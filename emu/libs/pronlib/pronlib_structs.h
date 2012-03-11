#ifndef __PRONLIB_STRUCTS_H__
#define __PRONLIB_STRUCTS_H__

#include <vesa_types.h>

struct PronWindowAttributes {
  unsigned short x;
  unsigned short y;
  unsigned short width;
  unsigned short height;
  color_t        bgColor;
};

#endif // __PRONLIB_STRUCTS_H__
