#include "widget.h"
#include "application.h"
#include "container.h"

namespace sombrero {

  Widget::Widget() {
    this->init();
  }

  Widget::~Widget() {
    pron::pronDestroyWindow(Application::getInstance()->d, this->pronWindow);
    Application::getInstance()->widgets.erase(this->pronWindow);
  }

  void Widget::init() {
    this->parent    = NULL;
    this->eventMask = 0;
    this->lastX      = -1;
    this->lastY      = -1;
    this->lastWidth  = -1;
    this->lastHeight = -1;
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

  void Widget::dontPropagateEvent(uint32_t eventMask) {
    this->dontPropagateEventMask |= PRON_EVENTMASK(eventMask);
    pron::pronDontPropagateEvent(Application::getInstance()->d, this->pronWindow,
              this->dontPropagateEventMask);
  }

  void Widget::propagateEvent(uint32_t eventMask) {
    this->dontPropagateEventMask &= ~PRON_EVENTMASK(eventMask);
    pron::pronDontPropagateEvent(Application::getInstance()->d, this->pronWindow,
              this->dontPropagateEventMask);
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
    if(this->lastWidth != this->width || this->lastHeight != this->height) {
      this->lastWidth  = this->width;
      this->lastHeight = this->height;
      this->resized();
      pronResizeWindow(Application::getInstance()->d, this->pronWindow, this->width, this->height);
    }
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
    if(this->lastX != this->x || this->lastY != this->y) {
      this->lastX = this->x;
      this->lastY = this->y;
      pronMoveWindowTo(Application::getInstance()->d, this->pronWindow, this->x, this->y);
    }
  }

  void Widget::update() {
    this->updatePronPosition();
    this->updatePronSize();
  }

  Container* Widget::getParent() {
    return this->parent;
  }

  void Widget::clear() {
    pron::pronClearWindow(Application::getInstance()->d, this->pronWindow);
  }

  void Widget::handleEventWindowCreated() {
  }

  void Widget::handleEventExpose() {
    //printf("handleEventExpose\n");
    this->draw();
  }

  void Widget::handleEventPointerMoved(pron::EventPointerMoved *mousePointerEvent) {
    this->mouseDrag(mousePointerEvent->xMove, mousePointerEvent->yMove,
                    mousePointerEvent->x, mousePointerEvent->y);
  }

  void Widget::handleEventMouseButton(pron::EventMouseButton *e __attribute__((unused))) {
    //printf("handleEventMouseButton\n");
  }

  void Widget::handleEventKeyPressed(pron::PronEvent *e __attribute__((unused))) {
  }

  void Widget::handleEventKeyReleased() {
  }

  void Widget::handleEventDestroyWindow() {
    // TODO : WTF ????
    exit(1);
  }

  void Widget::handleEventResizeWindow(int width, int height) {
    //printf("handleEventResizeWindow\n");
    bool isResized   = this->getWidth() != width || this->getHeight() != height;
    this->lastHeight = height;
    this->lastWidth  = width;
    if(isResized) {
      this->setWidth(width);
      this->setHeight(height);
      this->resized();
    }
  }

} // namespace sombrero
