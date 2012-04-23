#ifndef __CANVAS_H__
#define __CANVAS_H__
/**
 * @file canvas.h
 * Canvas class definition
 */

#include "widget.h"

class Widget;
class Container;

namespace sombrero {

/**
 * Canvas class definition. A container is a widget containing 
 * one widget
 */
class Canvas : public Widget {
  
 private:
  
 public:
  /**
   * Canvas constructor
   */
  Canvas(Container *parent);
  /**
   * Canvas destructor
   */
  ~Canvas();
  /**
   * Draws a ligne
   */
  void drawLine(int x1, int y1, int x2, int y2);
  /**
   * Drawing stuff
   */
  void draw();

};

} //namespace sombrero

#endif//__CANVAS_H__
