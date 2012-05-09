#include "widget.h"
#include "application.h"
#include "container.h"

namespace sombrero {

  Widget::Widget() {
    this->init();
  }

  Widget::Widget(Container *parent) {
    this->init();
    parent->add(this);
  }

  Widget::~Widget() {
    pron::pronDestroyWindow(Application::getInstance()->d, this->pronWindow);
  }

  void Widget::init() {
    this->parent    = NULL;
    this->eventMask = 0;
  }

  void Widget::subscribeEvent(uint32_t eventMask) {
    this->eventMask |= PRON_EVENTMASK(eventMask);
    pron::pronSelectInput(Application::getInstance()->d, this->pronWindow,
              this->eventMask);
  }

  void Widget::unsubscribeEvent(uint32_t eventMask) {
    this->eventMask &= ~PRON_EVENTMASK(eventMask);
    pron::pronSelectInput(Application::getInstance()->d, this->pronWindow,
              this->eventMask);
  }

  void Widget::setParent(Container *parent) {
    // TODO à réfléchir si on supprime si ça vaut pas null etc.
    if (this->parent == NULL) {
      this->parent = parent;
      // Creates the window
      this->pronWindow = pron::pronCreateWindow(Application::getInstance()->d,
                                                this->parent->pronWindow,
                                                this->x, this->y,
                                                this->width, this->height);
      // Associates the pron::window to the widget
      Application::getInstance()->widgets[this->pronWindow] = this;
      // Maps the window
      pron::pronMapWindow(Application::getInstance()->d, this->pronWindow);
      // Select events
      this->subscribeEvent(pron::EV_EXPOSE);
    }
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

  void Widget::updatePronSize() {
    pronResizeWindow(Application::getInstance()->d, this->pronWindow, this->width, this->height);
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
    if (visible) {
      pronMapWindow(Application::getInstance()->d, this->pronWindow);
    } else {
      pronUnmapWindow(Application::getInstance()->d, this->pronWindow);
    }
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

  void Widget::updatePronPosition() {
    pronMoveWindowTo(Application::getInstance()->d, this->pronWindow, this->x, this->y);
  }

  void Widget::update() {
    this->updatePronPosition();
    this->updatePronSize();
  }

  Container* Widget::getParent() {
    return this->parent;
  }

  void Widget::handleEventWindowCreated() {
  }

  void Widget::handleEventExpose() {
    this->draw();
  }

  void Widget::handleEventPointerMoved()  {
  }

  void Widget::handleEventMouseButton(pron::PronEvent *e __attribute__((unused))) {
    printf("handleEventMouseButton\n");
    this->clicked();
  }

  void Widget::handleEventKeyPressed(pron::PronEvent *e __attribute__((unused))) {
  }

  void Widget::handleEventKeyReleased() {
  }

  void Widget::handleEventDestroyWindow() {
    exit(1);
  }

  void Widget::handleEventResizeWindow(int width, int height) {
    this->setWidth(width);
    this->setHeight(height);
    this->resized();
  }

} // namespace sombrero
