#include "canvas.h"
#include "application.h"

namespace sombrero {

Canvas::Canvas(Container *parent)
  : Widget(parent) {
  this->pixmap = pronCreatePixmap(Application::getInstance()->d, this->getHeight(), this->getHeight(), SOMBRERO_CANVAS_DEPTH);
}

Canvas::~Canvas() {
  pronFreePixmap(Application::getInstance()->d, this->pixmap);
}

void Canvas::drawLine(int x1, int y1, int x2, int y2) {
  pronDrawLine(Application::getInstance()->d, this->pixmap, Application::getInstance()->d->defaultGC, x1, y1, x2, y2);
}

void Canvas::draw() {
  pronCopyArea(Application::getInstance()->d, this->pixmap, this->topWindow, Application::getInstance()->d->defaultGC, 0, 0, this->getWidth(), this->getHeight(), 0, 0);
}

} //namespace sombrero
