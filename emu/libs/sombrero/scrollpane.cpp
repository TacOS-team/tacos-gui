#include "hscrollpane.h"

static int factor = 100;

namespace sombrero {

ScrollPane::ScrollPane(Widget *widget) {
  this->add(widget);
}

void ScrollPane::init() {
  this->resizeWidget = true;
  this->setScrollBarWidth(20);
  this->scrollbar->setStep(factor*10);
  this->getWidget()->setX(0);
  this->scrollbar->newValue.connect(this, &ScrollPane::positionChanged);
  this->getWidget()->subscribeEvent(pron::EV_RESIZE_WINDOW);
  this->getWidget()->resized.connect((Widget*)this, &Widget::update);
}

void ScrollPane::setResizeWidget(bool resize) {
  this->resizeWidget = resize;
}

void ScrollPane::positionChanged(int val) {
  if(this->getWidget() && this->getWidgetLength() > this->getScrollPaneLength()) {
    this->setWidgetPosition(-1*val/factor);
    this->getWidget()->updatePronPosition();
  }
}

void ScrollPane::execUpdate() {
  //printf("ScrollPane::execUpdate\n");
  this->setScrollBarLength(this->getScrollPaneLength());
  this->moveScrollBarTo(this->getScrollPanewidth() - this->getScrollBarWidth());
  // If the widget has been set
  if(this->getWidget()) {
    if(this->resizeWidget) {
      this->setWidgetWidth(this->getScrollPanewidth() - this->getScrollBarWidth());
    }
    // If the widget is bigger than the scrollpane
    if(this->getWidgetLength() > this->getScrollPaneLength()) {
      // Sets the range. Is depends on the number of hidden pixels
      // Multiplied per factor to avoid strange behavior on the ratio of small values
      this->scrollbar->setRange(0, factor*(this->getWidgetLength() - this->getScrollPaneLength()));
      // The ratio depends on the ratio visible and hidden
      this->scrollbar->setRatio(factor*(this->getWidgetLength() - this->getScrollPaneLength())
        * ((float)this->getScrollPaneLength()/this->getWidgetLength()));
    } else {
      // If the widget is smaller, the scrollbar cannot be moved
      this->scrollbar->setRange(0, 0);
    }
    this->scrollbar->update();
    this->setWidgetPosition(-1*this->scrollbar->getValue()/factor);
    this->getWidget()->update();
  }
  Bin::execUpdate();
}


std::vector<Widget*> ScrollPane::getChildren() {
  std::vector<Widget*> res (Bin::getChildren());
  res.push_back(this->scrollbar);
  return res;
}

void ScrollPane::resetScroll() {
  this->scrollbar->setValue(0);
  this->positionChanged(0);
}

} // namespace sombrero
