#ifndef __CANVAS_H__
#define __CANVAS_H__
/**
 * @file canvas.h
 * Canvas class definition
 */

#include "widget.h"

#define SOMBRERO_CANVAS_DEPTH 24

class Container;

namespace sombrero {

/**
 * Canvas class definition. A container is a widget containing 
 * one widget
 */
class Canvas : public Widget {
  
 private:
  pron::Pixmap pixmap; /**< Pron pixmap : where to draw */
 public:
  /**
   * Canvas constructor
   * @param parent
   */
  Canvas(Container *parent);

  /**
   * Canvas destructor
   */
  ~Canvas();

  /**
   * Clears the canvas.
   */
  void clear();

  /**
   * Draws a ligne
   * @param x1
   * @param y1
   * @param x2
   * @param y2
   */
  void drawLine(int x1, int y1, int x2, int y2);

  /**
   * Draws a rectangle to (x,y) width height sized.
   * @param x The x-coordinate of the top-left corner of the rectangle
   * @param y The y-coordinate of the top-left corner of the rectangle
   * @param width The width of the rectangle
   * @param height The height of the rectangle
   */
  void drawRectangle(int x, int y, int width, int height);
  /**
   * Fills a rectangle to (x,y) width height sized.
   * @param x The x-coordinate of the top-left corner of the rectangle
   * @param y The y-coordinate of the top-left corner of the rectangle
   * @param width The width of the rectangle
   * @param height The height of the rectangle
   */
  void fillRectangle(int x, int y, int width, int height);
  /**
   * Draws a circle with given center (x,y) and radius.
   * @param x The x-coordinate of the top-left corner of the circle
   * @param y The y-coordinate of the top-left corner of the circle
   * @param radius The radius of the circle
   * @return success
   */
  void drawCircle(int x, int y, int radius);
  /**
   * Fills a circle with given center (x,y) and radius.
   * @param x The x-coordinate of the top-left corner of the circle
   * @param y The y-coordinate of the top-left corner of the circle
   * @param radius The radius of the circle
   * @return success
   */
  void fillCircle(int x, int y, int radius);
  /**
   * Drawing stuff
   */
  void draw();
};

} //namespace sombrero

#endif//__CANVAS_H__
