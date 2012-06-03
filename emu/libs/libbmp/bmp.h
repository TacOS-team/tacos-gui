#ifndef __BMP_H__
#define __BMP_H__
/**
 * @file bmp.h
 * BmpImage class definition
 */

#include <string>


/**
 * BmpImage class definition
 * @brief The BmpImage
 */
class BMPImageLoader {
 protected:
  struct readColor_t { unsigned char r, g, b; };
  struct writeColor_t { unsigned char r, g, b; };
  writeColor_t *buff;
 public:
  BMPImageLoader();
  int width;
  int height;
  char * load(const std::string &fileName);
  char * getBuffer();
};

#endif//__BMP_H__
