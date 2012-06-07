#include "vscrollbar.h"
#include "application.h"

namespace sombrero {

int VScrollBar::getMaxThumbLength() {
  return this->getHeight() - 2 * this->getMinThumbPosition();
}

void VScrollBar::execUpdate() {
  unsigned short thumbLength;
  unsigned short thumbPosition;
  this->getCursorInformations(thumbPosition, thumbLength);
  this->thumb.setX(this->marginSize);
  this->thumbPosition = thumbPosition;
  this->thumb.setY(this->thumbPosition);
  this->thumb.setWidth (this->getWidth() - 2*this->marginSize);
  this->thumb.setHeight(thumbLength);
  this->thumb.update();
  ScrollBar::execUpdate();
}


void VScrollBar::draw() {
  /*printf("draw vscrollbar (x, y, w, h) : %d, %d, %d, %d\n", this->getX(),
    this->getY(), this->getWidth(), this->getHeight());*/

  // Background under the scrollbar
  pron::pronFillRectangle(Application::getInstance()->d, this->pronWindow,
    this->bgGC, this->marginSize,this->getMinThumbPosition(),
    this->getWidth()-2*this->marginSize, this->thumbPosition-this->getMinThumbPosition());
  // Background above the scrollbar
  pron::pronFillRectangle(Application::getInstance()->d, this->pronWindow,
    this->bgGC, this->marginSize, this->thumbPosition+this->thumb.getHeight(),
    this->getWidth()-2*this->marginSize, this->getHeight()-this->thumbPosition-this->thumb.getHeight()-this->getMinThumbPosition());
  // Draws the margin
  for(int i = 0; i < this->marginSize; ++i) {
    pron::pronDrawRect(Application::getInstance()->d, this->pronWindow,
      this->bgGC, i,this->getMinThumbPosition()+i,
      this->getWidth()-2*i, this->getHeight()-2*this->getMinThumbPosition()-2*i);
  }
  // Background top button
  pron::pronFillRectangle(Application::getInstance()->d, this->pronWindow,
    this->scrollGC, 0,0,
    this->getWidth(), this->getMinThumbPosition()-1);
  pron::pronDrawLine(Application::getInstance()->d, this->pronWindow, this->fgGC,
    0, this->getMinThumbPosition()-1,
    this->getWidth(), this->getMinThumbPosition()-1);
  // Background bottom button
  pron::pronFillRectangle(Application::getInstance()->d, this->pronWindow,
    this->scrollGC, 0,this->getHeight()-this->getMinThumbPosition(),
    this->getWidth(), this->getMinThumbPosition());
  pron::pronDrawLine(Application::getInstance()->d, this->pronWindow, this->fgGC,
    0, this->getHeight()-this->getMinThumbPosition(),
    this->getWidth(), this->getHeight()-this->getMinThumbPosition());

  int buttonCenter = this->getWidth()/2;

  // Draws the arrows
  for(int i = 0; i < this->buttonSize - 3 && (i * 2 + 6 + this->marginSize) < this->getWidth(); ++i) {
    pron::pronDrawLine(Application::getInstance()->d, this->pronWindow, this->fgGC,
      buttonCenter - i, this->marginSize + 1 + i, 
      buttonCenter + i, this->marginSize + 1 + i);
    pron::pronDrawLine(Application::getInstance()->d, this->pronWindow, this->fgGC,
      buttonCenter - i, this->getHeight() - this->marginSize - 1 - i, 
      buttonCenter + i, this->getHeight() - this->marginSize - 1 - i);
  }
}

void VScrollBar::thumbClickedAt(int x __attribute__((unused)), int y) {
  this->clickPosition = y;
}

void VScrollBar::handleEventPointerMoved(pron::EventPointerMoved *mousePointerEvent) {
  ScrollBar::handleEventPointerMoved(mousePointerEvent);
  this->updateThumbPosition(mousePointerEvent->y - this->clickPosition);
}

void VScrollBar::handleClick(int x __attribute__((unused)), int y) {
  //printf("VScrollBar::handleEventMouseButton\n");
  // If the click is above the thumb we step up
  if(y < this->thumb.getY()) {
    int newVal = (int)this->value - this->step;
    if(newVal >= 0) {
      this->setValue(this->value - this->step);
    } else {
      this->value = this->min;
    }
  } else if(y > this->thumb.getY() + this->thumb.getHeight()) {
    // If below, we step down
    this->setValue(this->value + this->step);
  }
  this->update();
}

void VScrollBar::setThumbPosition(unsigned short position) {
  this->thumb.setY(position);
}

} // namespace sombrero