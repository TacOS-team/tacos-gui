#ifndef __PRONLIB_STRUCTS_H__
#define __PRONLIB_STRUCTS_H__

#include <vesa_types.h>
#include <cstdlib>

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
  PronImage(int width, int height, int format, char *data, int depth, int bytesPerPixel){
    this->width = width;
    this->height = height;
    this->format = format;
    this->data = data;
    this->depth = depth;
    this->freeBuffer = true;
    this->bytesPerPixel = bytesPerPixel;
  }

  PronImage(int width, int height, int format, char *data, int depth, int bytesPerPixel, bool freeBuffer){
    this->width = width;
    this->height = height;
    this->format = format;
    this->data = data;
    this->depth = depth;
    this->freeBuffer = freeBuffer;
    this->bytesPerPixel = bytesPerPixel;
  }

  ~PronImage(){
    if(this->freeBuffer){
      free(this->data);
    }
  }

  int width, height;// Size of the image
  int format;// XYBitmap, XYPixmap, ZPixmap
  char *data;// Pointer to image data
  int depth;// Depth of the image 24, 16, 8 bits
  bool freeBuffer;// True if we have to free the data buffer
  int bytesPerPixel;// Bytes per pixels
};

#endif // __PRONLIB_STRUCTS_H__
