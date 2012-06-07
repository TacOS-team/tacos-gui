#ifndef __SCROLLBAR_H__
#define __SCROLLBAR_H__
/**
 * @file scrollbar.h
 * Scrollbar class definition
 */

#include "container.h"
#include "button.h"

namespace sombrero {

/**
 * ScrollBar class definition
 * @brief The Scrollbar widget
 */ 
class ScrollBar : public Widget {

 protected:
  class Thumb : public Button {
   protected:
    void handleMouseDown(MouseButton button, int x, int y);
   public:
    signal2<int, int> downAt;
    /**
     * The default constructor 
     */
    Thumb();

  };

  pron::GC scrollGC; /**< The background GC */

  Thumb thumb;/** The thumb of the scrollbar */
  int thumbPosition;/** The current thumb position on the scrollbar */
  unsigned int value;/** The current value of the scrollbar */
  unsigned int min;/** The min value of the scrollbar */
  unsigned int max;/** The max value of the scrollbar */
  unsigned int ratio;/** The ratio the thumb spans */
  unsigned int step;/** The step added or removed from value for each click */
  unsigned short marginSize;/** The margin size of the scrollBar */
  unsigned short buttonSize;/** The maximum height of the buttons */
  int clickPosition;/** The last click position o nthe thum */

  /**
   * Computes the minimum thumb position from the margin and button sizes
   */
  int getMinThumbPosition();
  /**
   * Computes the maximum thumb position from the margin and button sizes
   */
  int getMaxThumbPosition(unsigned int thumbLength);
  /**
   * Abstract function. It sets the position of the thumb button.
   *  Sets the Y position for a vertical scrollbar or the X for an horizontal.
   */
  virtual void setThumbPosition(unsigned short position) = 0;
  /**
   * The max thumb length depends of the height for a vertical scrollbar
   *   or on the width for an horizontal
   */
  virtual int getMaxThumbLength() = 0;
  /**
   * Moves the thumb from the mouse move
   */
  virtual void updateThumbPosition(int move);
  // Slots
  void thumbClicked();/**  Slot when the thumb is clicked */
  virtual void thumbClickedAt(int x, int y) = 0;/**  Slot when the thumb is clicked */
  void thumbReleased();/** Slot when the thumb is released  */
 public:
  /**
   * Default constructor
   */
  ScrollBar();
  /**
   * Signal sent whent the value changed. The parameter is the new value
   */
  signal1<int> newValue;
  /**
   * Gets the curent value
   */
  int  getValue();
  /**
   * Sets the ratio the thumb spans
   */
  void setRatio(int ratio);
  /**
   * Gets the current ration the thumb spans
   */
  int  getRatio();
  /**
   * Set the step.
   *  The page step is the amount that the value changes by
   *   when the user presses the Page Up and Page Down keys
   */
  void setStep (unsigned int step);
  /**
   * Sets the new current value of the scrollbar
   */
  void setValue(unsigned int value);
  /**
   * Sets the minimum and maximum values of the scrollbar
   */
  void setRange(unsigned int min, unsigned int max);
  /**
   * Gets the position and length of the thumb
   */
  void getCursorInformations(unsigned short &position,
                             unsigned short &length);
  /**
   * Sets the new parent and creates the thumb button
   */
  virtual void setParent(Widget *parent);
  /**
   * Handle for a pointerMoved event
   */
  virtual void handleEventPointerMoved(pron::EventPointerMoved * e);
  /**
   * Handle for a mouseButton event
   * @param e A pointer to the event that accured
   */
  virtual void handleClick(int x, int y) = 0;
  /**
   * Draw the widget
   */
  virtual void draw()             = 0;
};

} // namespace sombrero

#endif//__SCROLLBAR_H__
