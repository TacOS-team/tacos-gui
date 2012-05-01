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
      this->pronWindow = pron::pronCreateWindow(Application::getInstance()->d, this->parent->pronWindow, this->x, this->y, this->width, this->height);
      // Associates the pron::window to the widget
      Application::getInstance()->widgets[this->pronWindow] = this;
      // Maps the window
      pron::pronMapWindow(Application::getInstance()->d, this->pronWindow);
      // Select events
      pron::pronSelectInput(Application::getInstance()->d, this->pronWindow, PRON_EVENTMASK(pron::EV_EXPOSE));
    }

  Widget::~Widget() {
    pron::pronDestroyWindow(Application::getInstance()->d, this->pronWindow);
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

  void Widget::handleEventWindowCreated () {
  }
  void Widget::handleEventExpose () {
    this->draw();
  }
  void Widget::handleEventPointerMoved ()  {
  }
  void Widget::handleEventMouseButton (pron::PronEvent * e __attribute__((unused))) {
    this->clicked();
  }

  void Widget::handleEventKeyPressed(pron::PronEvent * e __attribute__((unused))) {
  }
  void Widget::handleEventKeyReleased () {
  }
  void Widget::handleEventDestroyWindow ()  {
    exit(1);
  }
  void Widget::handleEventResizeWindow () {
  }

} // namespace sombrero
