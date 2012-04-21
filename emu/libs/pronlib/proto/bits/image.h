#ifndef __PRONPROTO_IMAGE_H__
#define __PRONPROTO_IMAGE_H__

#include <stdlib.h>

namespace pron {

/**
 * Image pixmap types
 */
enum PixmapType{
  XYBitmap = 0x0000, 
  XYPixmap = 0x0001, 
  ZPixmap  = 0X0002
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

} // namespace pron

#endif // __PRONPROTO_IMAGE_H__
