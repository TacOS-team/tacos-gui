#include "canvas.h"
#include "application.h"
#include "pronlib.h"

namespace sombrero {

Canvas::Canvas(int pWidth, int pHeight) : c(0, 0, 0), pWidth(pWidth), pHeight(pHeight) {
  this->pixmap = pronCreatePixmap(Application::getInstance()->d, pWidth, pHeight, SOMBRERO_CANVAS_DEPTH);
  this->pixmap2 = pronCreatePixmap(Application::getInstance()->d, pWidth, pHeight, SOMBRERO_CANVAS_DEPTH);
  Color bgColor(255, 255, 255);
  pron::PronGCValues values;
  values.fg = bgColor;
  this->bgGC = pronCreateGC(Application::getInstance()->d, values, pron::GC_VAL_FG);
  values.fg = c;
  this->fgGC = pronCreateGC(Application::getInstance()->d, values, pron::GC_VAL_FG);
  this->clear();
}

Canvas::~Canvas() {
  pronFreePixmap(Application::getInstance()->d, this->pixmap);
  pronFreePixmap(Application::getInstance()->d, this->pixmap2);
}

void Canvas::clear() {
  pronFillRectangle(Application::getInstance()->d, this->pixmap, this->bgGC, 0, 0, this->pWidth, this->pHeight);
  pronFillRectangle(Application::getInstance()->d, this->pixmap2, this->bgGC, 0, 0, this->pWidth, this->pHeight);
}

void Canvas::drawPoint(int x, int y){
  pronDrawPoint(Application::getInstance()->d, this->pixmap, this->fgGC, x, y);
}

void Canvas::drawLine(int x1, int y1, int x2, int y2) {
  pronDrawLine(Application::getInstance()->d, this->pixmap, this->fgGC, x1, y1, x2, y2);
}

void Canvas::drawRectangle(int x, int y, int width, int height) {
  pronDrawRect(Application::getInstance()->d, this->pixmap, this->fgGC, x, y, width, height);
}

void Canvas::fillRectangle(int x, int y, int width, int height) {
  pronFillRectangle(Application::getInstance()->d, this->pixmap, this->fgGC, x, y, width, height);
}

void Canvas::drawEllipse(int x0, int y0, int a, int b) {
  pronDrawEllipse(Application::getInstance()->d, this->pixmap, this->fgGC, x0, y0, a, b);
}

void Canvas::fillEllipse(int x0, int y0, int a, int b) {
  pronFillEllipse(Application::getInstance()->d, this->pixmap, this->fgGC, x0, y0, a, b);
}

void Canvas::draw() {
  pronCopyArea(Application::getInstance()->d, this->pixmap, this->pronWindow, this->fgGC, 0, 0, this->getWidth(), this->getHeight(), 0, 0);
}

void Canvas::setFGColor(const Color &c) {
  pron::PronGCValues values;
  values.fg = c;
  pron::pronChangeGC(Application::getInstance()->d, this->fgGC, values,
      pron::GC_VAL_FG);
}

void Canvas::switchPixmap() {
  pron::Pixmap tp;
  tp = this->pixmap;
  this->pixmap = this->pixmap2;
  this->pixmap2 = tp;
  this->restorePixmap();
}

void Canvas::restorePixmap() {
  pronCopyArea(Application::getInstance()->d, this->pixmap2, this->pixmap, this->fgGC, 0, 0, this->getWidth(), this->getHeight(), 0, 0);
}

} //namespace sombrero
