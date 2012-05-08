#include "canvas.h"
#include "application.h"
#include "pronlib.h"

namespace sombrero {

  Canvas::Canvas(Container *parent)
      : Widget(parent) {
    this->pixmap = pronCreatePixmap(Application::getInstance()->d, this->getWidth(), this->getHeight(), SOMBRERO_CANVAS_DEPTH);
    color_t bgColor;
    COLOR(bgColor, 24).r = (this->pixmap >> 16) << 3;
    COLOR(bgColor, 24).g = (this->pixmap >> 16) << 3;
    COLOR(bgColor, 24).b = (this->pixmap >> 16) << 3;
    pron::PronGCValues values;
    values.fg = bgColor;
    this->bgGC = pronCreateGC(Application::getInstance()->d, values, pron::GC_VAL_FG);
  }

  Canvas::~Canvas() {
    pronFreePixmap(Application::getInstance()->d, this->pixmap);
  }

  void Canvas::clear() {
    pronFillRectangle(Application::getInstance()->d, this->pixmap, this->bgGC, 0, 0, this->getWidth(), this->getHeight());
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

  void Canvas::drawEllipse(int x0, int y0, int a, int b) {
    pronDrawEllipse(Application::getInstance()->d, this->pixmap, Application::getInstance()->d->defaultGC, x0, y0, a, b);
  }

  void Canvas::fillEllipse(int x0, int y0, int a, int b) {
    pronFillEllipse(Application::getInstance()->d, this->pixmap, Application::getInstance()->d->defaultGC, x0, y0, a, b);
  }

  void Canvas::draw() {
    pronCopyArea(Application::getInstance()->d, this->pixmap, this->pronWindow, Application::getInstance()->d->defaultGC, 0, 0, this->getWidth(), this->getHeight(), 0, 0);
  }

} //namespace sombrero
