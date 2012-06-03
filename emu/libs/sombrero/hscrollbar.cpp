#include "hscrollbar.h"
#include "application.h"

namespace sombrero {

int HScrollBar::getMaxThumbLength() {
  return this->getWidth() - 2 * this->getMinThumbPosition();
}

void HScrollBar::execUpdate() {
  unsigned short thumbLength;
  unsigned short thumbPosition;
  this->getCursorInformations(thumbPosition, thumbLength);
  this->thumb.setY(this->marginSize);
  this->thumbPosition = thumbPosition;
  this->thumb.setX(this->thumbPosition);
  this->thumb.setHeight(this->getHeight() - 2*this->marginSize);
  this->thumb.setWidth(thumbLength);
  this->thumb.update();
  ScrollBar::execUpdate();
}


void HScrollBar::draw() {
  /*printf("draw hscrollbar (x, y, w, h) : %d, %d, %d, %d\n", this->getX(),
    this->getY(), this->getWidth(), this->getHeight());*/
  pron::pronClearWindow(Application::getInstance()->d, this->pronWindow);

  // Background of the scrollbar
  pron::pronFillRectangle(Application::getInstance()->d, this->pronWindow,
    Application::getInstance()->d->defaultGC, this->getMinThumbPosition(), 0,
    this->getMaxThumbLength(), this->getHeight());
  // Draws the margin
  pron::pronDrawRect(Application::getInstance()->d, this->pronWindow,
    Application::getInstance()->d->defaultGC, 0,0,
    this->getWidth(), this->getHeight());

  int buttonCenter = this->getHeight()/2;

  // Draws the arrows
  for(int i = 0; i < this->buttonSize - 3 && (i * 2 + 6 + this->marginSize) < this->getHeight(); ++i) {
    pron::pronDrawLine(Application::getInstance()->d, this->pronWindow, Application::getInstance()->d->defaultGC,
      this->marginSize + 1 + i, buttonCenter - i,
      this->marginSize + 1 + i, buttonCenter + i);
    pron::pronDrawLine(Application::getInstance()->d, this->pronWindow, Application::getInstance()->d->defaultGC,
      this->getWidth() - this->marginSize - 1 - i, buttonCenter - i,
      this->getWidth() - this->marginSize - 1 - i, buttonCenter + i);
  }
}

void HScrollBar::thumbClickedAt(int x __attribute__((unused)), int y) {
  this->clickPosition = x;
}

void HScrollBar::handleEventPointerMoved(pron::EventPointerMoved *mousePointerEvent) {
  ScrollBar::handleEventPointerMoved(mousePointerEvent);
  this->updateThumbPosition(mousePointerEvent->x - this->clickPosition);
}

void HScrollBar::handleClick(int x __attribute__((unused)), int y) {
  //printf("ScrollBar::handleEventMouseButton\n");
  // If the click is above the thumb we step up
  if(x < this->thumb.getX()) {
    int newVal = (int)this->value - this->step;
    if(newVal >= 0) {
      this->setValue(this->value - this->step);
    } else {
      this->value = this->min;
    }
  } else if(x > this->thumb.getX() + this->thumb.getWidth()) {
    // If below, we step down
    this->setValue(this->value + this->step);
  }
  this->update();
}

void HScrollBar::setThumbPosition(unsigned short position) {
  this->thumb.setX(position);
}

} // namespace sombrero