#include "vscrollpane.h"

static int factor = 100;

namespace sombrero {

VScrollPane::VScrollPane(Widget *widget) {
  this->add(widget);
  this->init();
}

void VScrollPane::init() {
  this->scrollBar.setWidth(20);
  this->scrollBar.setY(0);
  this->scrollBar.setStep(factor*10);
  this->getWidget()->setY(0);
  this->scrollBar.newValue.connect(this, &VScrollPane::YMoved);
}

void VScrollPane::YMoved(int val) {
  if(this->getWidget() && this->getWidget()->getHeight() > this->getHeight()) {
    this->getWidget()->setY(-1*val/factor);
    this->getWidget()->updatePronPosition();
  }
}

void VScrollPane::execUpdate() {
  //printf("update vscrollpane\n");
  this->scrollBar.setHeight(this->getHeight());
  this->scrollBar.setX(this->getWidth() - this->scrollBar.getWidth());
  // If the widget has been set
  if(this->getWidget()) {
    this->getWidget()->setWidth(this->getWidth() - this->scrollBar.getWidth());
    this->getWidget()->setX(0);
    // If the widget is bigger than the vscrollpane
    if(this->getWidget()->getHeight() > this->getHeight()) {
      // Sets the range. Is depends on the number of hidden pixels
      // Multiplied per factor to avoid strange behavior on the ratio of small values
      this->scrollBar.setRange(0, factor*(this->getWidget()->getHeight() - this->getHeight()));
      // The ratio depends on the ratio visible and hidden
      this->scrollBar.setRatio(factor*(this->getWidget()->getHeight() - this->getHeight())
        * ((float)this->getHeight()/this->getWidget()->getHeight()));
    } else {
      // If the widget is smaller, the scrollbar cannot be moved
      this->scrollBar.setRange(0, 0);
    }
    this->scrollBar.update();
    this->getWidget()->setY(-1*this->scrollBar.getValue()/factor);
    this->getWidget()->update();
  }
  Container::execUpdate();
}

void VScrollPane::setParent(Widget *parent) {
  Bin::setParent(parent);
  // If the widget is set, we create it
  if(this->getWidget()) {
    this->getWidget()->subscribeEvent(pron::EV_RESIZE_WINDOW);
    this->getWidget()->resized.connect((Widget*)this, &Widget::update);
  }
}

std::vector<Widget*> VScrollPane::getChildren() {
  std::vector<Widget*> res (Bin::getChildren());
  res.push_back(&this->scrollBar);
  return res;
}

} // namespace sombrero
