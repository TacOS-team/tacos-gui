#include "vscrollbar.h"
#include "application.h"

namespace sombrero {

int VScrollBar::getMaxThumbLength() {
  return this->getHeight() - 2 * this->getMinThumbPosition();
}

void VScrollBar::update() {
  unsigned short thumbLength;
  unsigned short thumbPosition;
  this->getCursorInformations(thumbPosition, thumbLength);
  this->thumb.setX(this->marginSize);
  this->thumbPosition = thumbPosition;
  this->thumb.setY(this->thumbPosition);
  this->thumb.setWidth (this->getWidth() - 2*this->marginSize);
  this->thumb.setHeight(thumbLength);
  this->thumb.update();
  ScrollBar::update();
}


void VScrollBar::draw() {
  /*printf("draw vscrollbar (x, y, w, h) : %d, %d, %d, %d\n", this->getX(),
    this->getY(), this->getWidth(), this->getHeight());*/
  pron::pronFillRectangle(Application::getInstance()->d, this->pronWindow,
    Application::getInstance()->d->defaultGC, 0,this->getMinThumbPosition(),
    this->getWidth(), this->getMaxThumbLength());
  pron::pronDrawRect(Application::getInstance()->d, this->pronWindow,
    Application::getInstance()->d->defaultGC, 0,0,
    this->getWidth(), this->getHeight());
}

void VScrollBar::handleEventPointerMoved(pron::EventPointerMoved *mousePointerEvent) {
  ScrollBar::handleEventPointerMoved(mousePointerEvent);
  /*if((this->thumbPosition > this->getMinThumbPosition()
          || mousePointerEvent->y > this->thumb.getHeight()/2)
      && (this->thumbPosition < this->getMaxThumbPosition(this->thumb.getHeight())
          || mousePointerEvent->y <= this->thumb.getHeight()/2)) {*/
    this->updateThumbPosition(mousePointerEvent->y - this->thumb.getHeight()/2);
  //}
}

void VScrollBar::setThumbPosition(unsigned short position) {
  this->thumb.setY(position);
}

} // namespace sombrero