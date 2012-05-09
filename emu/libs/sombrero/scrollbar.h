#ifndef __SCROLLBAR_H__
#define __SCROLLBAR_H__
/**
 * @file scrollpane.h
 * Button class definition
 */

#include "widget.h"

namespace sombrero {

/**
 * ScrollBar class definition
 * @brief The Scrollbar widget
 */ 
class ScrollBar : public Widget {

 protected:
  unsigned int min;
  unsigned int max;
  unsigned int thumb;
  ScrollBar();
 public:
  void setThumb(int thumb);
  void setRange(int min, int max);
  virtual void draw();
};

} // namespace sombrero

#endif//__SCROLLBAR_H__
