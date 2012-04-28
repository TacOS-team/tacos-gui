#ifndef __BUTTON_H__
#define __BUTTON_H__
/**
 * @file button.h
 * Button class definition
 */

#include "widget.h"

namespace sombrero {

class Container;

/**
 * Button class definition
 * @brief The window widget
 * Creates a window which can be decorated 
 * by guacamole
 */ 
class Button : public Widget {

 private:
  char *text;
  
 public:
  /**
   * The button constructor 
   * @param parent Parent Container
   */
  Button(Container *parent, char *text);
  /**
   * Button destructor
   */
  ~Button();
  /**
   * Button drawing stuff
   */
  void draw();

};

} // namespace sombrero

#endif//__BUTTON_H__
