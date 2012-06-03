#include "bin.h"
#include "container.h"

namespace sombrero {

Bin::Bin() {
  this->widget = NULL;
}

Bin::~Bin() {}

void Bin::add(Widget *widget) {
  this->widget = widget;
  widget->setParent(this);
}

void Bin::remove(Widget *widget) {
  if (this->widget == widget) {
    this->widget = NULL;
  }
}

Widget* Bin::getWidget() {
  return this->widget;
}

std::vector<Widget*> Bin::getChildren() {
  std::vector<Widget*> res;
  res.push_back(this->widget);
  return res;
}

} // Namespace sombrero
