#ifndef __VSCROLLPANE_H__
#define __VSCROLLPANE_H__
/**
 * @file vscrollpane.h
 * VScrollPane class definition
 */

#include "bin.h"
#include "vscrollbar.h"
#include "scrollpane.h"

namespace sombrero {

/**
 * VScrollPane class definition
 * @brief The VScrollpane container
 */ 
class VScrollPane : public ScrollPane {
  protected:
  void init();

 public:
  /**
   * Constructor
   */
  VScrollPane(Widget *widget);

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

#endif//__VSCROLLPANE_H__
