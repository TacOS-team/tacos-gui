#ifndef __SCROLLBAR_H__
#define __SCROLLBAR_H__
/**
 * @file scrollpane.h
 * Button class definition
 */

#include "container.h"
#include "button.h"

namespace sombrero {

/**
 * ScrollBar class definition
 * @brief The Scrollbar widget
 */ 
class ScrollBar : public Container, public has_slots<> {

 protected:
  Button thumb;
  int thumbPosition;
  unsigned int value;
  unsigned int min;
  unsigned int max;
  unsigned int ratio;
  unsigned int step;
  unsigned short marginSize;
  unsigned short buttonSize;
  ScrollBar();
  int getMinThumbPosition();
  int getMaxThumbPosition(unsigned int thumbLength);
  virtual void setThumbPosition(unsigned short position) = 0;
  virtual int getMaxThumbLength() = 0;
  virtual void updateThumbPosition(int move);
  // Slots
  void thumbClicked();
  void thumbReleased();
 public:
  signal1<int> newValue;
  int getValue();
  void setRatio(int ratio);
  void setStep (unsigned int step);
  void setValue(unsigned int value);
  void setRange(unsigned int min, unsigned int max);
  void getCursorInformations(unsigned short &position,
                             unsigned short &length);
  virtual void setParent(Container *parent);
  /**
   * Updates the widgets to pron
   */
  virtual void update();
  /**
   * Handle for a pointerMoved event
   */
  virtual void handleEventPointerMoved(pron::EventPointerMoved * e);
  /**
   * Handle for a mouseButton event
   * @param e A pointer to the event that accured
   */
  virtual void handleEventMouseButton(pron::EventMouseButton * e);
  virtual void draw()             = 0;
};

} // namespace sombrero

#endif//__SCROLLBAR_H__
