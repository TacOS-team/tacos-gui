#include "vscrollpane.h"
#include "application.h"

namespace sombrero {

VScrollPane::VScrollPane(Widget *widget) : ScrollPane(widget) {
  this->init();
}

void VScrollPane::init() {
  this->scrollbar = new VScrollBar();
  ScrollPane::init();
}

int VScrollPane::getScrollPaneLength() {
  return this->getHeight();
}

int VScrollPane::getScrollPanewidth() {
  return this->getWidth();
}

void VScrollPane::setScrollBarWidth(int width) {
  this->scrollbar->setWidth(width);
}

int VScrollPane::getScrollBarWidth() {
  return this->scrollbar->getWidth();
}

void VScrollPane::setScrollBarLength(int length) {
  this->scrollbar->setHeight(length);
}

int VScrollPane::getScrollBarLength() {
  return this->scrollbar->getHeight();
}

void VScrollPane::moveScrollBarTo(int position) {
  this->scrollbar->setY(0);
  this->scrollbar->setX(position);
}

void VScrollPane::setWidgetWidth(int width) {
  this->getWidget()->setWidth(width);
}

int VScrollPane::getWidgetLength() {
  return this->getWidget()->getHeight();
}

void VScrollPane::setWidgetPosition(int position) {
  this->getWidget()->setY(position);
  this->getWidget()->setX(0);
}

void VScrollPane::draw() {
  if(this->getWidget()->getHeight() < this->getHeight()) {
    pronFillRectangle(Application::getInstance()->d, this->pronWindow,
               this->bgGC,
               0, this->getWidget()->getHeight(),
               this->getWidth() - this->scrollbar->getWidth(), this->getHeight() - this->getWidget()->getHeight());
  }
  if(this->getWidget()->getWidth() < this->getWidth() - this->scrollbar->getWidth()) {
    pronFillRectangle(Application::getInstance()->d, this->pronWindow,
               this->bgGC,
               this->getWidget()->getWidth(), 0,
               this->getWidth() - this->scrollbar->getWidth() -this->getWidget()->getWidth(),
                min(this->getHeight(), this->getWidget()->getHeight()));
  }
}

} // namespace sombrero
