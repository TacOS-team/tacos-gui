#ifndef __HSCROLLPANE_H__
#define __HSCROLLPANE_H__
/**
 * @file vscrollpane.h
 * VScrollPane class definition
 */

#include "bin.h"
#include "hscrollbar.h"
#include "scrollpane.h"

namespace sombrero {

/**
 * HScrollPane class definition
 * @brief The Scrollpane container
 */ 
class HScrollPane : public ScrollPane {
  protected:
  void init();

 public:
  /**
   * Constructor
   */
  HScrollPane(Widget *widget);

  /**
   * Get the length of the ScrollPane (width for horizontal)
   */
  int getScrollPaneLength();
  /**
   * Get the width of the ScrollPane
   *   (on height for horizontal)
   */
  int getScrollPanewidth();
  /**
   * sets the scrollbarWidth
   */
  void setScrollBarWidth(int width);
  /**
   * gets the scrollbarWidth
   */
  int getScrollBarWidth();
  /**
   * sets the scrollbar Length
   */
  void setScrollBarLength(int length);
  /**
   * gets the scrollbar Length
   */
  int getScrollBarLength();
  /**
   * Moves the scrollbar to this position
   *   (on x or y depending if vertical or horizontal)
   */
  void moveScrollBarTo(int position);
  /**
   * Sets the widget width
   *   (on width or height depending if vertical or horizontal)
   */
  void setWidgetWidth(int width);
  /**
   * gets the widget length
   *   (on width or height depending if vertical or horizontal)
   */
  int getWidgetLength();
  /**
   * Sets the widget width
   *   (on width or height depending if vertical or horizontal)
   */
  void setWidgetPosition(int position);
};

} // namespace sombrero

#endif//__HSCROLLPANE_H__
