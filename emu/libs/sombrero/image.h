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
  char * rawImage; /**< the buffer where the uncompressed image is locally stored */
  string filename; /**< the name of the file to open */

  void init(); /**< intializes the image and the pixmap */

  /**
   * Get the location in rawImage of the c'th component of the pixel located at column i, line j
   * @param i The column number of the pixel
   * @param j The line number of the pixel
   * @param c The component of the pixel
   * @param currentWidth The width of the image stored in rawImage
   * @return The location in rawImage of the component
   */
  unsigned int getLocation(unsigned int i, unsigned int j, unsigned int c, unsigned int currentWidth);

   
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
  void rotate(bool clockwise);
  void applyNegativeFilter();
};

}// namespace sombrero

#endif//__IMAGE_H__
