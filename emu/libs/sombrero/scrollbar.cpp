#include "scrollbar.h"

namespace sombrero {

ScrollBar::ScrollBar() : thumb("oh yeah !!!") {
  this->ratio = 1;
  this->step  = 1;
  this->min   = 0;
  this->max   = 10;
  this->value = this->min;
  this->marginSize = 1;
  this->buttonSize = 10;
}

void ScrollBar::setRatio(int ratio) {
  this->ratio = ratio;
}

void ScrollBar::setStep(int step) {
  this->step = step;
}

void ScrollBar::setRange(unsigned int min, unsigned int max) {
  if(min > max) {
    min = max;
  }
  this->min = min;
  this->max = max;
}

void ScrollBar::thumbClicked() {
  printf("thumbClicked\n");
  this->subscribeEvent(pron::EV_POINTER_MOVED);
}

void ScrollBar::thumbReleased() {
  this->unsubscribeEvent(pron::EV_POINTER_MOVED);
}


void ScrollBar::setParent(Container *parent) {
  Widget::setParent(parent);
  this->thumb.setParent(this);
  this->thumb.clicked.connect  (this, &ScrollBar::thumbClicked);
  this->thumb.released.connect (this, &ScrollBar::thumbReleased);
  this->mouseDrag.connect(this, &ScrollBar::handleMouseMove);
}

int ScrollBar::getMinThumbPosition() {
  return this->buttonSize + this->marginSize;
}

int ScrollBar::getMaxThumbPosition() {
  return this->getMaxThumbLength();
}

void ScrollBar::update() {
  Widget::update();
}

void ScrollBar::updateThumbPosition(int move) {
  unsigned short thumbLength;
  unsigned short thumbPosition;
  this->getCursorInformations(thumbPosition, thumbLength);
  this->thumbPosition += move;
  if(this->thumbPosition < this->getMinThumbPosition()) {
    this->thumbPosition = this->getMinThumbPosition();
  } else if(this->thumbPosition > this->getMaxThumbPosition()) {
    this->thumbPosition = this->getMaxThumbPosition();
  }
  this->setThumbPosition(this->thumbPosition);
  this->thumb.updatePronPosition();
  unsigned int newValue = (float)(this->thumbPosition - this->getMinThumbPosition())
                / (this->getMaxThumbPosition() - this->getMinThumbPosition())
                *(this->max - this->min) + this->min;
  if(this->value != newValue) {
    this->newValue(newValue);
    this->value = newValue;
  }
}

void ScrollBar::getCursorInformations(unsigned short &thumbPosition,
                                      unsigned short &thumbLength) {
  unsigned int range = max - min;
  if(range == 0 || range <= this->ratio) {
    thumbPosition = 0;
    thumbLength   = this->getMaxThumbLength();
  } else {
    thumbLength   = this->getMaxThumbLength() * ((float)this->ratio / range);
    thumbPosition = (float)(this->value - min) / range;
  }
  thumbLength = (thumbLength < 15) ? 15 : thumbLength;
}

}