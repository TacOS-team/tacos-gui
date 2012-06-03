#ifndef __HSCROLLPANE_H__
#define __HSCROLLPANE_H__
/**
 * @file vscrollpane.h
 * VScrollPane class definition
 */

#include "bin.h"
#include "hscrollbar.h"

namespace sombrero {

/**
 * HScrollPane class definition
 * @brief The Scrollpane container
 */ 
class HScrollPane : public Bin {

 protected:
  /**
   * The horizontal scrollbar of the scrollpane
   */
  HScrollBar scrollBar;
  /**
   * Initialises the scrollpane
   */
  void init();
 public:
  /**
   * Constructor
   */
  HScrollPane(Widget *widget);

  /**
   * Sets the parent container
   * @param Pointer to the parent container
   */
  virtual void setParent(Widget *parent);
  /**
   * Updates the scrollbar etc.
   */
  void execUpdate();
  // Slots
  void XMoved(int val);/** Slot called when the user moves the scrollbar */
  
  std::vector<Widget*> getChildren();
};

} // namespace sombrero

#endif//__HSCROLLPANE_H__
