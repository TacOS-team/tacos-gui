#include "widget.h"
#include "application.h"
#include "container.h"

namespace sombrero {

Widget::Widget() {}

Widget::Widget(Container *parent)
    : parent(parent) {
  // On s'ajoute au container s'il n'est pas NULL
  parent->add(this);
  // Creates the window
  this->topWindow = pron::pronCreateWindow(Application::getInstance()->d, this->parent->topWindow, this->x, this->y, this->width, this->height);
  // Associates the pron::window to the widget
  Application::getInstance()->widgets[this->topWindow] = this;
  // Maps the window
  pron::pronMapWindow(Application::getInstance()->d, this->topWindow);
  // Select events
  pron::pronSelectInput(Application::getInstance()->d, this->topWindow, PRON_EVENTMASK(pron::EV_EXPOSE));
}

Widget::~Widget() {
  pron::pronDestroyWindow(Application::getInstance()->d, this->topWindow);
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

Container* Widget::getParent() {
  return this->parent;
}

void Widget::handleEvent(pron::PronEvent *e) {
  switch (e->type) {
    case pron::EV_EXPOSE: {
      this->draw();
      break;
    }
    case pron::EV_MOUSE_BUTTON: {
      if (((pron::EventMouseButton *)e)->b1) {
        this->clicked();
      }
      break;
    }
    case pron::EV_DESTROY_WINDOW:
      exit(1);
      break;
    default:
      break; 
  }
}

} // namespace sombrero
