#include "scrollbar.h"

namespace sombrero {

ScrollBar::ScrollBar() : thumb("o") {
  this->ratio = 1;
  this->step  = 1;
  this->min   = 0;
  this->max   = 10;
  this->value = this->min;
  this->marginSize = 2;
  this->buttonSize = 10;
}

int ScrollBar::getValue() {
  return this->value;
}

void ScrollBar::setRatio(int ratio) {
  this->ratio = ratio;
  // If the ration is too big, we set it at its maximum value
  if(this->ratio > this->max - this->min) {
    this->ratio = this->max - this->min;
  }
  this->update();
}

int ScrollBar::getRatio() {
  return this->ratio;
}

void ScrollBar::setStep(unsigned int step) {
  this->step = step;
}

void ScrollBar::setRange(unsigned int min, unsigned int max) {
  if(min > max) {
    min = max;
  }
  this->min = min;
  this->max = max;
  if(this->value < this->min) {
    this->value = min;
  } else if(this->value > this->max) {
    this->value = this->max;
  }
  if(this->ratio > this->max - this->min) {
    this->ratio = this->max - this->min;
  }
  if(this->ratio == 0) {
    this->ratio = 1;
  }
  this->update();
}

void ScrollBar::thumbClicked() {
  // TODO save position cursor on the thumb
  //   to keep this position during the move avoiding
  //   to center the mouse each time

  // Subscribe to mouse events to move the thumb
  this->subscribeEvent(pron::EV_POINTER_MOVED);
}

void ScrollBar::thumbReleased() {
  // Unsubscribe to mouse events because the thumb is released
  this->unsubscribeEvent(pron::EV_POINTER_MOVED);
}


void ScrollBar::setParent(Widget *parent) {
  Widget::setParent(parent);
  this->thumb.setParent(this);
  this->thumb.clicked.connect  (this, &ScrollBar::thumbClicked);
  this->thumb.released.connect (this, &ScrollBar::thumbReleased);
  this->thumb.dontPropagateEvent(pron::EV_MOUSE_BUTTON);
  this->subscribeEvent(pron::EV_MOUSE_BUTTON);
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

void ScrollBar::updateThumbPosition(int move) {
  unsigned short thumbLength;
  unsigned short thumbPosition;
  // Gets the current thumb informations
  this->getCursorInformations(thumbPosition, thumbLength);
  // Adds the mouse move to the position
  this->thumbPosition += move;
  // Check if the values are still valid
  if(this->thumbPosition < this->getMinThumbPosition()) {
    this->thumbPosition = this->getMinThumbPosition();
  } else if(this->thumbPosition > this->getMaxThumbPosition(thumbLength)) {
    this->thumbPosition = this->getMaxThumbPosition(thumbLength);
  }
  // Sets the new position
  this->setThumbPosition(this->thumbPosition);
  // Sends the new position to pron
  this->thumb.updatePronPosition();
  // Compute the new current value
  unsigned int newValue = (float)(this->thumbPosition - this->getMinThumbPosition())
                / (this->getMaxThumbPosition(thumbLength) - this->getMinThumbPosition())
                *(this->max - this->min) + this->min;
  // If the value changed, we send a signal
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
    // Computes the thumb length depending to the ratio
    thumbLength = this->getMaxThumbLength() * (float)this->ratio / (range + 1);
    // Sets a minimum length of the thumb
    thumbLength = (thumbLength < 15) ? 15 : thumbLength;
    // Computes the position from the new length
    thumbPosition = (float)(this->value - min) / range
                    * (this->getMaxThumbPosition(thumbLength)-this->getMinThumbPosition());
  }
  thumbPosition += this->getMinThumbPosition();
}

}
