#include "hscrollpane.h"

static int factor = 100;

namespace sombrero {

HScrollPane::HScrollPane(Widget *widget) {
  this->add(widget);
  this->init();
}

void HScrollPane::init() {
  this->scrollBar.setHeight(20);
  this->scrollBar.setX(0);
  this->scrollBar.setStep(factor*10);
  this->getWidget()->setX(0);
  this->scrollBar.newValue.connect(this, &HScrollPane::XMoved);
}

void HScrollPane::XMoved(int val) {
  if(this->getWidget() && this->getWidget()->getWidth() > this->getWidth()) {
    this->getWidget()->setX(-1*val/factor);
    this->getWidget()->updatePronPosition();
  }
}

void HScrollPane::execUpdate() {
  //printf("HScrollPane::execUpdate\n");
  this->scrollBar.setWidth(this->getWidth());
  this->scrollBar.setY(this->getHeight() - this->scrollBar.getHeight());
  // If the widget has been set
  if(this->getWidget()) {
    this->getWidget()->setHeight(this->getHeight() - this->scrollBar.getHeight());
    this->getWidget()->setY(0);
    // If the widget is bigger than the hscrollpane
    if(this->getWidget()->getWidth() > this->getWidth()) {
      // Sets the range. Is depends on the number of hidden pixels
      // Multiplied per factor to avoid strange behavior on the ratio of small values
      this->scrollBar.setRange(0, factor*(this->getWidget()->getWidth() - this->getWidth()));
      // The ratio depends on the ratio visible and hidden
      this->scrollBar.setRatio(factor*(this->getWidget()->getWidth() - this->getWidth())
        * ((float)this->getWidth()/this->getWidget()->getWidth()));
    } else {
      // If the widget is smaller, the scrollbar cannot be moved
      this->scrollBar.setRange(0, 0);
    }
    this->scrollBar.update();
    this->getWidget()->setX(-1*this->scrollBar.getValue()/factor);
    this->getWidget()->update();
  }
  Container::execUpdate();
}

void HScrollPane::setParent(Widget *parent) {
  Bin::setParent(parent);
  // If the widget is set, we create it
  if(this->getWidget()) {
    this->getWidget()->subscribeEvent(pron::EV_RESIZE_WINDOW);
    this->getWidget()->resized.connect((Widget*)this, &Widget::update);
  }
}

std::vector<Widget*> HScrollPane::getChildren() {
  std::vector<Widget*> res (Bin::getChildren());
  res.push_back(&this->scrollBar);
  return res;
}

} // namespace sombrero
