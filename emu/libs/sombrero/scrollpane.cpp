#include "scrollpane.h"

namespace sombrero {


ScrollPane::ScrollPane(Widget *widget) {
  this->widget = widget;
  this->init();
}

void ScrollPane::init() {
  this->scrollBar.setWidth(30);
  this->scrollBar.setY(0);
  this->scrollBar.setStep(10);
  this->widget->setY(0);
  this->scrollBar.newValue.connect(this, &ScrollPane::YMoved);
}

void ScrollPane::YMoved(int val) {
  if(this->widget && this->widget->getHeight() > this->getHeight()) {
    this->widget->setY(-1*val);
    this->widget->updatePronPosition();
  }
}


void ScrollPane::update() {
  printf("update scrollpane\n");
  this->scrollBar.setHeight(this->getHeight());
  this->scrollBar.setX(this->getWidth() - this->scrollBar.getWidth());
  if(this->widget) {
    this->widget->setWidth(this->getWidth() - this->scrollBar.getWidth());
    this->widget->setX(0);
    if(this->widget->getHeight() > this->getHeight()) {
      this->scrollBar.setRange(0, this->widget->getHeight() - this->getHeight());
      this->scrollBar.setRatio((this->widget->getHeight() - this->getHeight())
        * ((float)this->getHeight()/this->widget->getHeight()));
    } else {
      this->scrollBar.setRange(0, 0);
    }
    this->widget->setY(this->scrollBar.getValue());
    this->widget->update();
  }
  Container::update();
}

void ScrollPane::setParent(Container *parent) {
  Container::setParent(parent);
  this->scrollBar.setParent(this);
  if(this->widget) {
    this->widget->setParent(this);
    this->widget->subscribeEvent(pron::EV_RESIZE_WINDOW);
  }
}

} // namespace sombrero
