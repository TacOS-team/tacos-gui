#include "canvas.h"

namespace sombrero {

Canvas::Canvas(Container *parent)
  : Widget(parent) {
}

Canvas::~Canvas() {

}

void Canvas::drawLine(int x1, int y1, int x2, int y2) {
  x1++;//XXX: WARNING
  x2++;
  y1++;
  y2++;
}

void Canvas::draw() {
}

} //namespace sombrero
