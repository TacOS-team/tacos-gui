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

 private:
  unsigned char fontSize;
  std::string text;

  void init();
  
 public:
  /**
   * The button constructor 
   * @param parent Parent Container
   */
  Button(const char *text);
  /**
   * The button constructor 
   * @param parent Parent Container
   */
  Button(const  std::string text);
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
