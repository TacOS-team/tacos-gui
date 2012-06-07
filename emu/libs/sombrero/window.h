#ifndef __WINDOW_H__
#define __WINDOW_H__
/**
 * @file window.h
 * Window class definition
 */

#include <string>
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
  
 public:
  /**
   * Window init
   * @param title The title of the window (displayed by the WM)
   * @param x
   * @param y
   * @param width
   * @param height
   * @param decorate Whether the WM should decorate the window or not
   */
  void init(std::string title, int x, int y, int width, int height, bool decorate);
  /**
   * The window constructor 
   * @param title The title of the window (displayed by the WM)
   * @param x
   * @param y
   * @param width
   * @param height
   * @param decorate Whether the WM should decorate the window or not
   */
  Window(std::string title, int x, int y, int width, int height, bool decorate = true);
  /**
   * Window destructor
   */
  ~Window();
  /**
   * Handle for a resizeWindow event
   */
  virtual void handleEventResizeWindow(int width, int height) ;
  /**
   * Adds a widget in the children vector
   * @param widget Pointer to the widget to add
   */
  void add(Widget *widget);
};
 
} // namespace sombrero

#endif//__WINDOW_H__
