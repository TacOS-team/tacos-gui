#include "container.h"
#include "widget.h"
#include <vector>
#include <algorithm>

namespace sombrero {

Container::Container() {}

Container::~Container() {
}

void Container::update() {
  Widget::update();
}

} //namespace sombrero

