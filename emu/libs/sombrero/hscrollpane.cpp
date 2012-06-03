#include "hscrollpane.h"

namespace sombrero {

HScrollPane::HScrollPane(Widget *widget) : ScrollPane(widget) {
  this->init();
}

void HScrollPane::init() {
  this->scrollbar = new HScrollBar();
  ScrollPane::init();
}

int HScrollPane::getScrollPaneLength() {
  return this->getWidth();
}

int HScrollPane::getScrollPanewidth() {
  return this->getHeight();
}

void HScrollPane::setScrollBarWidth(int width) {
  this->scrollbar->setHeight(width);
}

int HScrollPane::getScrollBarWidth() {
  return this->scrollbar->getHeight();
}

void HScrollPane::setScrollBarLength(int length) {
  this->scrollbar->setWidth(length);
}

int HScrollPane::getScrollBarLength() {
  return this->scrollbar->getWidth();
}

void HScrollPane::moveScrollBarTo(int position) {
  this->scrollbar->setX(0);
  this->scrollbar->setY(position);
}

void HScrollPane::setWidgetWidth(int width) {
  this->getWidget()->setHeight(width);
}

int HScrollPane::getWidgetLength() {
  return this->getWidget()->getWidth();
}

void HScrollPane::setWidgetPosition(int position) {
  this->getWidget()->setX(position);
  this->getWidget()->setY(0);
}

} // namespace sombrero
