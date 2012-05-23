#ifndef __SCROLLPANE_H__
#define __SCROLLPANE_H__
/**
 * @file scrollpane.h
 * ScrollPane class definition
 */

#include "bin.h"
#include "vscrollbar.h"

namespace sombrero {

/**
 * ScrollPane class definition
 * @brief The Scrollpane container
 */ 
class ScrollPane : public Bin, public has_slots<> {

 protected:
  /**
   * The vertical scrollbar of the scrollpane
   */
  VScrollBar scrollBar;
  /**
   * The widget inside the scrollpane
   */
  Widget *widget;
  /**
   * Initialises the scrollpane
   */
  void init();
 public:
  /**
   * Constructor
   */
  ScrollPane(Widget *widget);

  /**
   * Sets the parent container
   * @param Pointer to the parent container
   */
  virtual void setParent(Container *parent);
  /**
   * Updates the scrollbar etc.
   */
  virtual void update();
  // Slots
  void YMoved(int val);/** Slot called when the user moves the scrollbar */
};

} // namespace sombrero

#endif//__SCROLLPANE_H__
