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
  pron::Pixmap pixmap; /**< Pron pixmap : the current pixmap, where to draw */
  pron::Pixmap pixmap2; /**< Pron pixmap : two pixmaps, to restore things  */
  pron::GC bgGC; /**< GC to use when clearing the pixmap */

 public:

  /**
   * Canvas constructor
   * @param parent
   * @param pixmap pwidth
   * @param pixmap pheight
   */
  Canvas(int pWidth, int pHeight);

  /**
   * Canvas destructor
   */
  ~Canvas();

  /**
   * Clears the canvas.
   */
  void clear();

  /**
   * Switches the current pixmap where to draw
   * copying the current pixmap in the other 
   */
  void switchPixmap();

  /**
   * Restore the pixmap of the non current pixmap 
   * in the current pixmap
   */
  void restorePixmap();

  /**
   * Draws a ligne
   * @param x1
   * @param y1
   * @param x2
   * @param y2
   */
  void drawLine(int x1, int y1, int x2, int y2);

  /**
   * Draws a point at (x, y).
   * @param x The x-coordinate of the point
   * @param y The y-coordinate of the point
   * @return success
   */
  void drawPoint(int x, int y);

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
   * Draws an ellipse with given center (x0,y0) and radiuses a and b.
   * @param x0 The x-coordinate of the center of the ellipse
   * @param y0 The y-coordinate of the center of the ellipse
   * @param a The longest radius of the ellipse
   * @param b The shortest radius of the ellipse
   */
  void drawEllipse(int x0, int y0, int a, int b);

  /**
   * Draws a filled ellipse with given center (x0,y0) and radiuses a and b.
   * @param x0 The x-coordinate of the center of the ellipse
   * @param y0 The y-coordinate of the center of the ellipse
   * @param a The longest radius of the ellipse
   * @param b The shortest radius of the ellipse
   */
  void fillEllipse(int x0, int y0, int a, int b);

  /**
   * Drawing stuff
   */
  void draw();
};

} //namespace sombrero

#endif//__CANVAS_H__
