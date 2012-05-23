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
   * Sets the parent container
   * @param Pointer to the parent container
   */
  virtual void setParent(Container *parent);
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
   * Handle for a mouseButton event
   * @param e A pointer to the event that accured
   */
  virtual void handleEventMouseButton(pron::EventMouseButton * e); 

};

} // namespace sombrero

#endif//__BUTTON_H__
