#ifndef __VSCROLLBAR_H__
#define __VSCROLLBAR_H__
/**
 * @file vscrollbar.h
 * VScrollBar class definition
 */

#include "scrollbar.h"

namespace sombrero {

/**
 * VScrollBar class definition
 * @brief The VScrollBar widget
 */ 
class VScrollBar : public ScrollBar {

 protected:
  /**
   * Gets the max thumb length
   */
  virtual int getMaxThumbLength();
  /**
   * Sets the position of the thumb (Y position for a vertical scrollbar)
   */
  virtual void setThumbPosition(unsigned short position);
 public:
  /**
   * Draws the scrollbar
   */
  virtual void draw();
  /**
   * Updates the widgets to pron
   */
  virtual void update();
  /**
   * Handle for a pointerMoved event
   */
  virtual void handleEventPointerMoved(pron::EventPointerMoved * e) ;
  /**
   * Handle for a mouseButton event
   * @param e A pointer to the event that accured
   */
  virtual void handleEventMouseButton(pron::EventMouseButton * e);
};

} // namespace sombrero

#endif//__VSCROLLBAR_H__
