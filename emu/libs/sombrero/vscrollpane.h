#ifndef __VSCROLLPANE_H__
#define __VSCROLLPANE_H__
/**
 * @file vscrollpane.h
 * VScrollPane class definition
 */

#include "bin.h"
#include "vscrollbar.h"

namespace sombrero {

/**
 * ScrollPane class definition
 * @brief The Scrollpane container
 */ 
class VScrollPane : public Bin {

 protected:
  /**
   * The vertical scrollbar of the scrollpane
   */
  VScrollBar scrollBar;
  /**
   * Initialises the scrollpane
   */
  void init();
 public:
  /**
   * Constructor
   */
  VScrollPane(Widget *widget);

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
  void YMoved(int val);/** Slot called when the user moves the scrollbar */
  
  std::vector<Widget*> getChildren();
};

} // namespace sombrero

#endif//__VSCROLLPANE_H__
