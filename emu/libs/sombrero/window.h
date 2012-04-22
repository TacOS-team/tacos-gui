#ifndef __WINDOW_H__
#define __WINDOW_H__

#include "widget.h"

/**
 * @file window.h
 * Window class definition
 */

namespace sombrero {

class Widget;

/**
 * Window class definition
 * @brief The window widget
 * Creates a window which can be decorated 
 * by guacamole
 */ 
class Window : public Widget {

 private:
  
 public:
  /**
   * The window constructor 
   * @param x
   * @param y
   * @param width
   * @param height
   */
  Window(int x, int y, int width, int height);
  ~Window();
  /**
   * Draws the window
   */
  void draw();
  
};
 
} // namespace sombrero

#endif//__WINDOW_H__
