#include "container.h"
#include "widget.h"
#include <vector>
#include <algorithm>

namespace sombrero {

Container::Container() {}

Container::~Container() {}

void Container::add(Widget *widget) {
  this->children.push_back(widget);
  widget->setParent(this);
}

void Container::remove(Widget *widget) {
  this->children.erase(std::find(this->children.begin(), this->children.end(), widget));
}

} //namespace sombrero

