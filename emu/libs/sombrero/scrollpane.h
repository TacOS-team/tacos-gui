#ifndef __SCROLLPANE_H__
#define __SCROLLPANE_H__
/**
 * @file vscrollpane.h
 * VScrollPane class definition
 */

#include "bin.h"
#include "hscrollbar.h"

namespace sombrero {

/**
 * ScrollPane class definition
 * @brief The abstract Scrollpane container
 */ 
class ScrollPane : public Bin {

 protected:
  ScrollBar *scrollbar;
  bool resizeWidget;
  /**
   * Initialises the scrollpane
   */
  virtual void init();
 public:
  /**
   * Constructor
   */
  ScrollPane(Widget *widget);

  /**
   * Asks to the scrollpane to resize the widget or not
   *   default : true
   * @param resize true if the widget has to be resized
   */
  void setResizeWidget(bool resize);

  /**
   * Updates the scrollbar etc.
   */
  void execUpdate();
  // Slots
  void positionChanged(int val);/** Slot called when the user moves the scrollbar */

  std::vector<Widget*> getChildren();

  /**
   * Get the length of the ScrollPane
   */
  virtual int getScrollPaneLength() = 0;
  /**
   * Get the width of the ScrollPane
   *   (on width or height depending if vertical or horizontal)
   */
  virtual int getScrollPanewidth() = 0;
  /**
   * sets the scrollbarWidth
   */
  virtual void setScrollBarWidth(int width) = 0;
  /**
   * gets the scrollbarWidth
   */
  virtual int getScrollBarWidth() = 0;
  /**
   * sets the scrollbar Length
   */
  virtual void setScrollBarLength(int length) = 0;
  /**
   * gets the scrollbar Length
   */
  virtual int getScrollBarLength() = 0;
  /**
   * Moves the scrollbar to this position
   *   (on x or y depending if vertical or horizontal)
   */
  virtual void moveScrollBarTo(int position) = 0;
  /**
   * Sets the widget width
   *   (on width or height depending if vertical or horizontal)
   */
  virtual void setWidgetWidth(int width) = 0;
  /**
   * gets the widget length
   *   (on width or height depending if vertical or horizontal)
   */
  virtual int getWidgetLength() = 0;
  /**
   * Sets the widget width
   *   (on width or height depending if vertical or horizontal)
   */
  virtual void setWidgetPosition(int position) = 0;
  /**
   * Resets the value of the scrollbar
   * @TODO DEUGUE !!!
   */
  void resetScroll();
  
};

} // namespace sombrero

#endif//__SCROLLPANE_H__
