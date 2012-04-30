#include "canvas.h"
#include "application.h"
#include "pronlib.h"

namespace sombrero {

Canvas::Canvas(Container *parent)
    : Widget(parent) {
  this->pixmap = pronCreatePixmap(Application::getInstance()->d, this->getWidth(), this->getHeight(), SOMBRERO_CANVAS_DEPTH);
}

Canvas::~Canvas() {
  pronFreePixmap(Application::getInstance()->d, this->pixmap);
}
  
void Canvas::clear() {
  pronClearWindow(Application::getInstance()->d, this->pixmap);
}

void Canvas::drawPoint(int x, int y){
  pronDrawPoint(Application::getInstance()->d, this->pixmap, Application::getInstance()->d->defaultGC, x, y);
}

void Canvas::drawLine(int x1, int y1, int x2, int y2) {
  pronDrawLine(Application::getInstance()->d, this->pixmap, Application::getInstance()->d->defaultGC, x1, y1, x2, y2);
}

void Canvas::drawRectangle(int x, int y, int width, int height) {
  pronDrawRect(Application::getInstance()->d, this->pixmap, Application::getInstance()->d->defaultGC, x, y, width, height);
}

void Canvas::fillRectangle(int x, int y, int width, int height) {
  pronFillRectangle(Application::getInstance()->d, this->pixmap, Application::getInstance()->d->defaultGC, x, y, width, height);
}

void Canvas::drawCircle(int x, int y, int radius) {
  pronDrawCircle(Application::getInstance()->d, this->pixmap, Application::getInstance()->d->defaultGC, x, y, radius);
}

void Canvas::fillCircle(int x, int y, int radius) {
  pronFillCircle(Application::getInstance()->d, this->pixmap, Application::getInstance()->d->defaultGC, x, y, radius);
}

void Canvas::draw() {
  pronCopyArea(Application::getInstance()->d, this->pixmap, this->topWindow, Application::getInstance()->d->defaultGC, 0, 0, this->getWidth(), this->getHeight(), 0, 0);
}

} //namespace sombrero
