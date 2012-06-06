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

  /*
   * Send a pixmap to pron
   * @param createNew Indiquates if we need to allocate a new pixamp in pron or not 
   */
  void sendPixmap (bool createNew);

  /*
   * Calculate the new value of the component for the fonk filter
   * @param i the colomn number of the pixel
   * @param j the line number of the pixel
   * @param c the component of the pixel
   * @param raw the raw image from where calculate
   */
  char calculateNewComp (unsigned int i, unsigned int j, unsigned int c, char * raw);
   
 public:
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
   * Rotate the image of 90° or -90°
   * @param clockwise Indiquates whether we rotate 90° or -90°
   */
  void rotate(bool clockwise);

  /*
   * Reverse colors of the image pixels
   */
  void applyNegativeFilter();
  
  /*
   * apply an awesome filter to the image
   */
  void applyPowerfullnessOfTheFonkFilter();

  /*
   * Get the pixmap of the image
   * @return The pron pixmap to return
   */
  pron::Pixmap getPixMap();

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

  /**
   * Set attribute xOffset
   * @param newXOffset The new xOffset
   */
  void setXOffset(int newXOffset);
  
  /**
   * Set attribute xOffset
   * @param newXOffset The new xOffset
   */
  void setYOffset(int newYOffset);
};

}// namespace sombrero

#endif//__IMAGE_H__
