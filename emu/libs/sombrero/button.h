#ifndef __BUTTON_H__
#define __BUTTON_H__
/**
 * @file button.h
 * Button class definition
 */

#include "widget.h"
#include <string>

namespace sombrero {

class Container;

/**
 * Button class definition
 * @brief The Button widget
 */ 
class Button : public Widget {

 protected:
  unsigned char fontSize;
  std::string text;

  void init();

  void handleMouseDown(MouseButton button, int x, int y);
  void handleMouseReleased(MouseButton button, int x, int y);
  
 public:
  // Signals
  signal0<> clicked;
  signal0<> released;
  /**
   * The default button constructor
   */
  Button();
  /**
   * The button constructor 
   * @param parent Parent Container
   */
  Button(const char *text);
  /**
   * The button constructor 
   * @param parent Parent Container
   */
  Button(const std::string &text);
  /**
   * Button destructor
   */
  ~Button();
  /**
   * Gets the text of the button
   */
  virtual std::string getText();
  /**
   * Sets the text of the button
   */
  void setText(std::string text);
  /**
   * Button drawing stuff
   */
  virtual void draw();

};

} // namespace sombrero

#endif//__BUTTON_H__
