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
  Button increaseButton;
  Button decreaseButton;
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
  int getMaxThumbPosition();
  virtual void setThumbPosition(unsigned short position) = 0;
  virtual int getMaxThumbLength() = 0;
  virtual void updateThumbPosition(int move);
  // Slots
  void thumbClicked();
  void thumbReleased();
  void increaseClicked();
  void decreaseClicked();
 public:
  signal1<int> newValue;
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
  virtual void handleMouseMove(int xMove, int yMove) = 0;
  virtual void draw()             = 0;
};

} // namespace sombrero

#endif//__SCROLLBAR_H__
