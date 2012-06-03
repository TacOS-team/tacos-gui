#ifndef __DOUBLE_SCROLLPANE_H__
#define __DOUBLE_SCROLLPANE_H__
/**
 * @file doublescrollpane.h
 * DoubleScrollPane class definition
 */

#include "sombrero.h"

namespace sombrero {

/**
 * DoubleScrollPane class definition
 * @brief The abstract DoubleScrollpane container
 */ 
class DoubleScrollPane : public Bin {

 protected:
  VScrollBar *vscrollbar;
  HScrollBar *hscrollbar;
  Widget *maskWidget;
  /**
   * Initialises the doublescrollpane
   */
  virtual void init();
 public:
  /**
   * Constructor
   */
  DoubleScrollPane(Widget *widget);

  /**
   * Updates the scrollbar etc.
   */
  void execUpdate();
  // Slots
  void xPositionChanged(int val);/** Slot called when the user moves the scrollbar */
  void yPositionChanged(int val);/** Slot called when the user moves the scrollbar */

  std::vector<Widget*> getChildren();

  void draw();
  
};

} // namespace sombrero

#endif//__DOUBLE_SCROLLPANE_H__
