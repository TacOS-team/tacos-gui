#ifndef __PRONLIB_STRUCTS_H__
#define __PRONLIB_STRUCTS_H__

#include <vesa_types.h>
#include <cstdlib>

/**
 * Structure for pron window attributes
 */
struct PronWindowAttributes {
  unsigned short x;
  unsigned short y;
  unsigned short width;
  unsigned short height;
  color_t        bgColor;
};

/**
 * Structure for pron graphics contexts values
 */
struct PronGCValues {
  color_t fg;
  color_t bg;
};

/**
 * Structure for pron Image
 * This structure is free data pointer
 */
struct PronImage {
  PronImage(int width, int height, int format, char *data, int depth){
    this->width = width;
    this->height = height;
    this->format = format;
    this->data = data;
    this->depth = depth;
  }

  ~PronImage(){
    free(this->data);
  }

  int width, height;// Size of the image
  int format;// XYBitmap, XYPixmap, ZPixmap
  char *data;// Pointer to image data
  int depth;// Depth of the image 24, 16, 8 bits
};

#endif // __PRONLIB_STRUCTS_H__
