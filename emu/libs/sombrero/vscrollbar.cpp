#include "vscrollbar.h"
#include "application.h"

namespace sombrero {

VScrollBar::VScrollBar() {
  
}

int VScrollBar::getMaxThumbLength() {
  return this->getHeight() - 2 * this->getMinThumbPosition();
}

void VScrollBar::update() {
  unsigned short thumbLength;
  unsigned short thumbPosition;
  this->getCursorInformations(thumbPosition, thumbLength);
  this->thumb.setX(this->marginSize);
  this->thumbPosition = this->getMinThumbPosition() + thumbPosition;
  this->thumb.setY(this->thumbPosition);
  this->thumb.setWidth (this->getWidth()  - 2*this->marginSize);
  this->thumb.setHeight(thumbLength);
  this->thumb.update();
  ScrollBar::update();
}


void VScrollBar::draw() {
  /*printf("draw vscrollbar (x, y, w, h) : %d, %d, %d, %d\n", this->getX(), this->getY(),
   this->getWidth(), this->getHeight());*/
  //pron::pronClearWindow(Application::getInstance()->d, this->pronWindow);
}

void VScrollBar::handleMouseMove(int xMove __attribute__((unused)), int yMove) {
  this->updateThumbPosition(yMove);
}

void VScrollBar::setThumbPosition(unsigned short position) {
  this->thumb.setY(position);
}

} // namespace sombrero