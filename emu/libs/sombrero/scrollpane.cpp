#include "scrollpane.h"

namespace sombrero {


ScrollPane::ScrollPane(Widget *widget) {
  this->widget = widget;
  this->init();
}

void ScrollPane::init() {
  this->scrollBar.setWidth(30);
  this->scrollBar.setY(0);
  this->widget->setY(0);
  this->scrollBar.newValue.connect(this, &ScrollPane::YMoved);
}

void ScrollPane::YMoved(int val) {
  if(this->widget) {
    this->widget->setY(-1*val);
    this->widget->updatePronPosition();
  }
}

void ScrollPane::update() {
  this->scrollBar.setHeight(this->getHeight());
  this->scrollBar.setX(this->getWidth() - this->scrollBar.getWidth());
  if(this->widget) {
    this->widget->setWidth(this->getWidth() - this->scrollBar.getWidth());
    this->widget->setX(0);
    if(this->widget->getHeight() > this->getHeight()) {
      this->scrollBar.setRange(0, this->widget->getHeight() - this->getHeight());
    }
    this->widget->update();
  }
  this->scrollBar.update();
  Bin::update();
}

void ScrollPane::setParent(Container *parent) {
  Bin::setParent(parent);
  this->scrollBar.setParent(this);
  if(this->widget) {
    this->widget->setParent(this);
  }
}

} // namespace sombrero
