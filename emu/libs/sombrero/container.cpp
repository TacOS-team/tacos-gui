#include "container.h"
#include "widget.h"
#include <vector>

namespace sombrero {

Container::Container(Container *parent, int x, int y, int width, int height) 
  : Widget (parent, x, y, width, height) {
}

Container::Container(int x, int y, int width, int height) 
  : Widget (x, y, width, height) {
}

Container::~Container() {
  
}

void Container::add(Widget *widget) {
  this->children.push_back(widget);
}

void Container::remove(Widget *widget) {
  vector<Widget*>::iterator it;
  for (it = this->children.begin(); it < this->children.end(); it++) {
    if (*it == widget) {
      this->children.erase(it, it);
    }
  }    
}

std::vector<Widget*> Container::getChildren() {
  return this->children;
}

} //namespace sombrero

