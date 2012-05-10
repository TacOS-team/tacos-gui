#ifndef __VSCROLLBAR_H__
#define __VSCROLLBAR_H__
/**
 * @file scrollpane.h
 * Button class definition
 */

#include "scrollbar.h"

namespace sombrero {

/**
 * VScrollBar class definition
 * @brief The VScrollBar widget
 */ 
class VScrollBar : public ScrollBar {

 protected:
  virtual int getMaxThumbLength();
  virtual void setThumbPosition(unsigned short position);
 public:
  VScrollBar();
  virtual void draw();
  /**
   * Updates the widgets to pron
   */
  virtual void update();
  void handleMouseMove(int xMove, int yMove);
};

} // namespace sombrero

#endif//__VSCROLLBAR_H__
