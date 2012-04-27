#include "container.h"
#include "widget.h"
#include <vector>
#include <algorithm>

namespace sombrero {

Container::Container() {}

Container::Container(Container *parent)
    : Widget(parent) {
}

Container::~Container() {
}

void Container::add(Widget *widget) {
  this->children.push_back(widget);
}

void Container::remove(Widget *widget) {
  this->children.erase(std::find(this->children.begin(), this->children.end(), widget));
}

void Container::drawChildren() {
  vector<Widget*>::iterator it;
  for (it = this->children.begin(); it < this->children.end(); ++it) {
    (*it)->draw();
  }
}

void Container::draw() {
  this->drawChildren();
}

std::vector<Widget*> Container::getChildren() {
  return this->children;
}

} //namespace sombrero

