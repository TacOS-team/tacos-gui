#ifndef __WINDOW_H__
#define __WINDOW_H__
/**
 * @file window.h
 * Window class definition
 */

#include "bin.h"

namespace sombrero {

class Bin;

/**
 * Window class definition
 * @brief The window widget
 * Creates a window which can be decorated 
 * by guacamole
 */ 
class Window : public Bin {

 protected:
  Container *layout;
  
 public:
  /**
   * The window constructor 
   * @param x
   * @param y
   * @param width
   * @param height
   */
  Window(int x, int y, int width, int height);
  /**
   * Window destructor
   */
  ~Window();
  /**
   * Handle for a resizeWindow event
   */
  virtual void handleEventResizeWindow(int width, int height) ;
  /**
   * Sets the layout of the window
   * @param Pointer to the layout
   */
  void setLayout(Container *layout);
  /**
   * Adds a widget in the children vector
   * @param widget Pointer to the widget to add
   */
  void add(Widget *widget);
};
 
} // namespace sombrero

#endif//__WINDOW_H__
