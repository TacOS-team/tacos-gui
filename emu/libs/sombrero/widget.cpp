#include "widget.h"
#include "application.h"

namespace sombrero {

Widget::Widget(int x, int y, int width, int height) 
  : x(x), y(y), width(width), height(height) {
  this->topWindow = pron::pronCreateWindow(Application::getInstance()->d, Application::getInstance()->d->rootWindow, this->x, this->y, this->width, this->height);
}

Widget::~Widget() {
  pron::pronDestroyWindow(Application::getInstance()->d, this->topWindow);
}

pron::Window Widget::getTopWindow() {
  return this->topWindow;
}

int Widget::getWidth() {
  return this->width;
}

void Widget::setWidth(int width) {
  this->width = width;
}

int Widget::getHeight() {
  return this->height;
}

void Widget::setHeight(int height) {
  this->height = height;
}

bool Widget::isActive() {
  return this->active;
}

void Widget::setActive(bool active) {
  this->active = active;
}
bool Widget::isVisible() {
  return this->visible;
}

void Widget::setVisible(bool visible) {
  this->visible = visible;
}

int Widget::getX() {
  return this->x;
}

void Widget::setX(int x) {
  this->x = x;
}

int Widget::getY() {
  return this->y;
}

void Widget::setY(int y) {
  this->y = y;
}

} // namespace sombrero
