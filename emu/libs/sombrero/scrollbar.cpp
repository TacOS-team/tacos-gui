#include "scrollbar.h"

namespace sombrero {

ScrollBar::ScrollBar() : thumb("oh yeah !!!"), increaseButton("vvvvvv"), decreaseButton("^^^^^^") {
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
  if(this->ratio > this->max - this->min) {
    this->ratio = this->max - this->min;
  }
  this->update();
}

void ScrollBar::setStep(unsigned int step) {
  this->step = step;
  if(this->step > this->max - this->min) {
    this->step = this->max - this->min;
  }
  this->update();
}

void ScrollBar::setRange(unsigned int min, unsigned int max) {
  if(min > max) {
    min = max;
  }
  this->min = min;
  this->max = max;
  this->value = min;
  if(this->ratio > this->max - this->min) {
    this->ratio = this->max - this->min;
  }
  this->update();
}

void ScrollBar::thumbClicked() {
  this->subscribeEvent(pron::EV_POINTER_MOVED);
}

void ScrollBar::thumbReleased() {
  this->unsubscribeEvent(pron::EV_POINTER_MOVED);
}

void ScrollBar::increaseClicked() {
  this->setValue(this->value + this->step);
  this->update();
}

void ScrollBar::decreaseClicked() {
  if((int)this->value - (int)this->step < 0) {
    this->setValue(this->min);
  } else {
    this->setValue(this->value - this->step);
  }
  this->update();
}


void ScrollBar::setParent(Container *parent) {
  Widget::setParent(parent);
  this->thumb.setParent(this);
  this->increaseButton.setParent(this);
  this->decreaseButton.setParent(this);
  this->thumb.clicked.connect  (this, &ScrollBar::thumbClicked);
  this->thumb.released.connect (this, &ScrollBar::thumbReleased);
  this->increaseButton.clicked.connect (this, &ScrollBar::increaseClicked);
  this->decreaseButton.clicked.connect (this, &ScrollBar::decreaseClicked);
}

void ScrollBar::setValue(unsigned int value) {
  unsigned int oldVal = this->value;
  this->value = value;
  if(this->value > this->max) {
    this->value = this->max;
  } else if(this->value < this->min) {
    this->value = this->min;
  }
  if(oldVal != this->value) {
    this->newValue(this->value);
  }
}

int ScrollBar::getMinThumbPosition() {
  return this->buttonSize + this->marginSize;
}

int ScrollBar::getMaxThumbPosition(unsigned int thumbLength) {
  return this->getMaxThumbLength() + this->getMinThumbPosition() - thumbLength;
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
  } else if(this->thumbPosition > this->getMaxThumbPosition(thumbLength)) {
    this->thumbPosition = this->getMaxThumbPosition(thumbLength);
  }
  this->setThumbPosition(this->thumbPosition);
  this->thumb.updatePronPosition();
  unsigned int newValue = (float)(this->thumbPosition - this->getMinThumbPosition())
                / (this->getMaxThumbPosition(thumbLength) - this->getMinThumbPosition())
                *(this->max - this->min) + this->min;
  if(this->value != newValue) {
    this->setValue(newValue);
  }
}

void ScrollBar::handleEventPointerMoved(pron::EventPointerMoved *mousePointerEvent) {
  Widget::handleEventPointerMoved(mousePointerEvent);
}

void ScrollBar::getCursorInformations(unsigned short &thumbPosition,
                                      unsigned short &thumbLength) {
  unsigned int range = max - min;
  if(range == 0) {
    thumbPosition = 0;
    thumbLength = this->getMaxThumbLength();
  } else {
    thumbLength = this->getMaxThumbLength() * (float)this->ratio / (range + 1);
    thumbLength = (thumbLength < 15) ? 15 : thumbLength;
    thumbPosition = (float)(this->value - min) / range
                    * (this->getMaxThumbPosition(thumbLength)-this->getMinThumbPosition());
  }
  thumbPosition += this->getMinThumbPosition();
}

}