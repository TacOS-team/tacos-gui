#ifndef __IMAGE_H__
#define __IMAGE_H__
/**
 * @file image.h
 * Image class definition
 */

#include <string>
#include <jpeglib.h>
#include "widget.h"

namespace sombrero {

/**
 * Image class definition
 * @brief The Image widget
 */ 
class Image : public Widget {
 private:
  unsigned int imageWidth; /**< The real image width */
  unsigned int imageHeight; /**< The real image height */
  unsigned int nbComponents; /**< The number of components per pixel */
  int xOffset; /**< the X offset of the top left corner */
  int yOffset; /**< the Y offset of the top left corner */
  pron::Pixmap pixmap; /**< The pixmap where the image is put */
  char * raw_image; /**< the buffer where the uncompressed image is locally stored */
  string filename; /**< the name of the file to open */
  void init(); /**< intializes the image and the pixmap */
   
 public:
  pron::Pixmap getPixMap();
  /**
   * Image constructor
   * @param filename The name of the file to open
   */
  Image(const string &filename);
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

  void setXOffset(int newXOffset);
  void setYOffset(int newYOffset);
  void applyNegativeFilter();
};

}// namespace sombrero

#endif//__IMAGE_H__
