#include "bin.h"
#include "container.h"

namespace sombrero {

Bin::Bin() {
  this->full = 0;
}

Bin::Bin(Container *parent)
    : Container(parent) {
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
