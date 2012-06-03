#include "container.h"
#include "widget.h"
#include <vector>
#include <algorithm>

namespace sombrero {

Container::Container() {}

Container::~Container() {}

void Container::setParent(Widget *widget) {
  Widget::setParent(widget);
  vector<Widget*> children = this->getChildren();
  for(vector<Widget*>::iterator it = children.begin(); it != children.end(); ++it) {
    (*it)->setParent(this);
  }
}

} //namespace sombrero

