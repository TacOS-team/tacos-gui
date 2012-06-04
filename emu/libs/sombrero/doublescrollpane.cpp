#include "doublescrollpane.h"

static const int   factor = 100;
static const float step   = 0.1;

namespace sombrero {

DoubleScrollPane::DoubleScrollPane(Widget *widget) {
  this->add(widget);
  this->init();
}

void DoubleScrollPane::init() {
  this->vscrollbar = new VScrollBar();
  this->hscrollbar = new HScrollBar();
  this->maskWidget = new Widget();
  this->vscrollbar->setWidth(20);
  this->hscrollbar->setHeight(20);
  this->vscrollbar->newValue.connect(this, &DoubleScrollPane::yPositionChanged);
  this->hscrollbar->newValue.connect(this, &DoubleScrollPane::xPositionChanged);
  this->getWidget()->subscribeEvent(pron::EV_RESIZE_WINDOW);
  this->getWidget()->resized.connect((Widget*)this, &Widget::update);
}


void DoubleScrollPane::xPositionChanged(int val) {
  if(this->getWidget() && this->getWidth() > this->getWidth() - this->vscrollbar->getWidth()) {
    this->getWidget()->setX(-1*val/factor);
    this->getWidget()->updatePronPosition();
  }
}

void DoubleScrollPane::yPositionChanged(int val) {
  if(this->getWidget() && this->getHeight() > this->getHeight() - this->hscrollbar->getHeight()) {
    this->getWidget()->setY(-1*val/factor);
    this->getWidget()->updatePronPosition();
  }
}

void DoubleScrollPane::execUpdate() {
  this->vscrollbar->setHeight(this->getHeight() - this->hscrollbar->getHeight());
  this->vscrollbar->setX(this->getWidth() - this->vscrollbar->getWidth());
  this->vscrollbar->setY(0);
  this->hscrollbar->setWidth(this->getWidth() - this->vscrollbar->getWidth());
  this->hscrollbar->setY(this->getHeight() - this->hscrollbar->getHeight());
  this->hscrollbar->setX(0);
  if(this->getWidget()) {
    // XPosition
    int maxWidth = this->getWidth() - this->vscrollbar->getWidth();
    int widgetWidth = this->getWidget()->getWidth();
    if(this->getWidget()->getWidth() > maxWidth) {
      int max = factor*(widgetWidth - maxWidth);
      this->hscrollbar->setRange(0, max);
      this->hscrollbar->setRatio(max * ((float)maxWidth/widgetWidth));
      this->hscrollbar->setStep(max*step);
    } else {
      // If the widget is smaller, the scrollbar cannot be moved
      this->hscrollbar->setRange(0, 0);
    }
    this->hscrollbar->update();
    this->getWidget()->setX(-1*this->hscrollbar->getValue()/factor);
    // YPosition
    int maxHeight = this->getHeight() - this->hscrollbar->getHeight();
    int widgetHeight = this->getWidget()->getHeight();
    if(this->getWidget()->getHeight() > maxHeight) {
      int max = factor*(widgetHeight - maxHeight);
      this->vscrollbar->setRange(0, max);
      this->vscrollbar->setRatio(max * ((float)maxHeight/widgetHeight));
      this->vscrollbar->setStep(max*step);
    } else {
      // If the widget is smaller, the scrollbar cannot be moved
      this->vscrollbar->setRange(0, 0);
    }
    this->vscrollbar->update();
    this->getWidget()->setY(-1*this->vscrollbar->getValue()/factor);
    this->getWidget()->update();
  }
  this->maskWidget->setX(this->hscrollbar->getWidth());
  this->maskWidget->setY(this->vscrollbar->getHeight());
  this->maskWidget->setWidth(this->vscrollbar->getWidth());
  this->maskWidget->setHeight(this->hscrollbar->getHeight());
  this->maskWidget->update();
  Bin::execUpdate();
}


std::vector<Widget*> DoubleScrollPane::getChildren() {
  std::vector<Widget*> res (Bin::getChildren());
  res.push_back(this->vscrollbar);
  res.push_back(this->hscrollbar);
  res.push_back(this->maskWidget);
  return res;
}

void DoubleScrollPane::draw() {
  // Right of the widget
  if(this->getWidget()->getWidth() < this->getWidth() - this->vscrollbar->getWidth()) {
    pronFillRectangle(Application::getInstance()->d, this->pronWindow,
               this->bgGC,
               this->getWidget()->getWidth(), 0,
               this->getWidth() - this->vscrollbar->getWidth() - this->getWidget()->getWidth(),
                this->getHeight() - this->hscrollbar->getHeight());
  }
  // Bottom of the widget
  if(this->getWidget()->getHeight() < this->getHeight() - this->hscrollbar->getHeight()) {
    pronFillRectangle(Application::getInstance()->d, this->pronWindow,
               this->bgGC,
               0, this->getWidget()->getHeight(),
               min(this->getWidth() - this->vscrollbar->getWidth(), this->getWidget()->getWidth()),
                this->getHeight() - this->hscrollbar->getHeight() - this->getWidget()->getHeight());
  }
  // Small rectangle at the bottom right
  this->maskWidget->clear();
}

} // namespace sombrero
