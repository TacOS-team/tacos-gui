#include "scrollbar.h"

namespace sombrero {

ScrollBar::ScrollBar() {
  this->thumb = 1;
  this->min   = 0;
  this->max   = 10;
}

void ScrollBar::setThumb(int thumb) {
  this->thumb = thumb;
}

void ScrollBar::setRange(int min, int max) {
  if(min > max) {
    min = max;
  }
  this->min = min;
  this->max = max;
}

void ScrollBar::draw() {

}

}