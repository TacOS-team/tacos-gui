#ifndef __IMAGE_H__
#define __IMAGE_H__
/**
 * @file image.h
 * Image class definition
 */

#include "widget.h"
#include <string>
#include <jpeglib.h>

namespace sombrero {
/**
 * Image class definition
 * @brief The Image widget
 */ 
class Image : public Widget {
 private:
  unsigned int imageWidth; /**< The real image width */
  unsigned int imageHeight; /**< The real image height */
  int xOffset; /**< the X offset of the top left corner */
  int yOffset; /**< the Y offset of the top left corner */
  bool isReversed; /**< incates whether the colors of the image are reversed */
  pron::Pixmap pixmap; /**< The pixmap where the image is put */
  string &filename; /**< the name of the file to open */
  void init(); /**< intializes the image and the pixmap */
   
 public:
  /**
   * Image constructor
   * @param filename The name of the file to open
   */
  Image(string &filename);
  /**
   * Image destructor
   */
  ~Image();
  /**
   * Image drawing stuff
   */
  void draw();

  /**
   * Gets the width of the image
   * @return the width of the image
   */
  unsigned int getImageWidth();
  /**
   * Gets the width of the image
   * @return the height of the image
   */
  unsigned int getImageHeight();

  void reverseColors();

  void setXOffset(int newXOffset);
  void setYOffset(int newYOffset);
};

}// namespace sombrero

#endif//__IMAGE_H__
