#ifndef __SCROLLPANE_H__
#define __SCROLLPANE_H__
/**
 * @file scrollpane.h
 * Button class definition
 */

#include "container.h"
#include "vscrollbar.h"

namespace sombrero {

/**
 * ScrollPane class definition
 * @brief The Scrollpane container
 */ 
class ScrollPane : public Container, public has_slots<> {

 protected:
  VScrollBar scrollBar;
  Widget *widget;
  void init();
 public:
  ScrollPane(Widget *widget);

  /**
   * Sets the parent container
   * @param Pointer to the parent container
   */
  virtual void setParent(Container *parent);
  virtual void update();
  // Slots
  void YMoved(int val);
};

} // namespace sombrero

#endif//__SCROLLPANE_H__
