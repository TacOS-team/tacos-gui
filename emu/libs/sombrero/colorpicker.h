#ifndef __COLORPICKER__
#define __COLORPICKER__
/**
 * @file colorpicker.h
 * ColorPicker class definition
 */

#include "grid.h"
#include "color.h"
#include "label.h"

namespace sombrero {

class ColorButton;

class ColorPicker : public Grid {
 private:
  int nbColors; /**< The number of buttons per color */
  ColorButton **bcolors; /**< The button colors [0; nbColors[ : red, [nbColors; 2nbColors[ : green, [2nbColors; 3nbColors[ : blue */
  Label lcolor; /**< Label displaying the coosen color */
  Color c; /**< The current color */

 protected:
 public:
  // Signals
  signal1<Color> colorChanged; /**< notifies chen color is changed */
  /**
   * Default constructor
   * @param nbColors the number of buttons per color
   */
  ColorPicker(unsigned int nbColors);
  /**
   * Default desctructor
   */
  ~ColorPicker();
  /**
   * Inits the ColorPicker
   */
  void init();
  /**
   * Sets the current color and send colorChanged signal
   * @param c current color
   * @param c colorComponent
   */
  void notifyCurrentColor(Color c, int colorComponent);
  /**
   * Sets the current color without notifiying users
   * @param c The color
   */
  void setCurrentColor(Color c);
};

} // namespace sombrero

#endif//__COLORPICKER__
