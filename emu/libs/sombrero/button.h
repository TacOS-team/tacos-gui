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
  std::string text;

  void init();

  void handleMouseDown(MouseButton button, int x, int y);
  void handleMouseReleased(MouseButton button, int x, int y);

  bool isDown;

  void setDown();
  void setReleased();
  
 public:
  // Signals
  signal0<> clicked;
  signal0<> down;
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
  /**
   * Function called when a client clicks on the widget
   * @param x the x position of the mouse
   * @param y the y position of the mouse
   */
  void handleClick(int x, int y);

};

} // namespace sombrero

#endif//__BUTTON_H__
