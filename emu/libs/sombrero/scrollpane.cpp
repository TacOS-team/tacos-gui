#include "scrollpane.h"

int factor = 100;

namespace sombrero {

ScrollPane::ScrollPane(Widget *widget) {
  this->widget = widget;
  this->init();
}

void ScrollPane::init() {
  this->scrollBar.setWidth(30);
  this->scrollBar.setY(0);
  this->scrollBar.setStep(factor*10);
  this->widget->setY(0);
  this->scrollBar.newValue.connect(this, &ScrollPane::YMoved);
}

void ScrollPane::YMoved(int val) {
  if(this->widget && this->widget->getHeight() > this->getHeight()) {
    this->widget->setY(-1*val/factor);
    this->widget->updatePronPosition();
  }
}

void ScrollPane::execUpdate() {
  //printf("update scrollpane\n");
  this->scrollBar.setHeight(this->getHeight());
  this->scrollBar.setX(this->getWidth() - this->scrollBar.getWidth());
  // If the widget has been set
  if(this->widget) {
    this->widget->setWidth(this->getWidth() - this->scrollBar.getWidth());
    this->widget->setX(0);
    // If the widget is bigger than the scrollpane
    if(this->widget->getHeight() > this->getHeight()) {
      // Sets the range. Is depends on the number of hidden pixels
      // Multiplied per factor to avoid strange behavior on the ratio of small values
      this->scrollBar.setRange(0, factor*(this->widget->getHeight() - this->getHeight()));
      // The ratio depends on the ratio visible and hidden
      this->scrollBar.setRatio(factor*(this->widget->getHeight() - this->getHeight())
        * ((float)this->getHeight()/this->widget->getHeight()));
    } else {
      // If the widget is smaller, the scrollbar cannot be moved
      this->scrollBar.setRange(0, 0);
    }
    this->scrollBar.update();
    this->widget->setY(this->scrollBar.getValue());
    this->widget->update();
  }
  Container::execUpdate();
}

void ScrollPane::setParent(Widget *parent) {
  Bin::setParent(parent);
  this->scrollBar.setParent(this);
  // If the widget is set, we create it
  if(this->widget) {
    this->widget->setParent(this);
    this->widget->subscribeEvent(pron::EV_RESIZE_WINDOW);
    this->widget->resized.connect((Widget*)this, &Widget::update);
  }
}

} // namespace sombrero
