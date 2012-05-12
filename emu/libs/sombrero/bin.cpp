#include "bin.h"
#include "container.h"

namespace sombrero {

Bin::Bin() {
  this->isFull = false;
}

Bin::Bin(Container *parent)
    : Container(parent) {
  this->isFull = false;
}

Bin::~Bin() {
}

void Bin::add(Widget *widget) {
  if (!this->isFull) {
    Container::add(widget);
    this->isFull = true;
  }
}

void Bin::remove(Widget *widget) {
  if (this->isFull) {
    Container::remove(widget);
    this->isFull = false;
  }
}

Widget * Bin::getWidget() {
  if(this->isFull) {
    return this->getChildren()[0];
  }
  return NULL;
}

} // Namespace sombrero
