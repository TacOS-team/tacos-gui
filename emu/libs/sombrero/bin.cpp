#include "bin.h"
#include "container.h"

namespace sombrero {

Bin::Bin(Container *parent, int x, int y, int width, int height)
  : Container(parent, x, y, width, height) {
  this->full = 0;
}

Bin::Bin(int x, int y, int width, int height)
  : Container(x, y, width, height) {
  this->full = 0;
}

Bin::~Bin() {
}

void Bin::add(Widget *widget) {
  if (!this->full) {
    Container::add(widget);
    this->full = 1;
  }
}

void Bin::remove(Widget *widget) {
  if (this->full) {
    Container::remove(widget);
    this->full = 0;
  }
}
  

} // Namespace sombrero
