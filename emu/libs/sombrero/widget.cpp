#include "widget.h"
#include "application.h"

namespace sombrero {

Widget::Widget() {
  this->init();
  // Initialiazes GC
  Color cb(0, 0, 0);
  Color cw(255, 255, 255);
  pron::PronGCValues values;
  values.bg = cw;
  values.fg = cb;
  this->fgColor = pron::pronCreateGC(Application::getInstance()->d, values, pron::GC_VAL_FG | pron::GC_VAL_BG);
  values.bg = cw;
  values.fg = cb;
  this->bgColor = pron::pronCreateGC(Application::getInstance()->d, values, pron::GC_VAL_FG | pron::GC_VAL_BG);
  /*pron::PronWindowAttributes newAttr;
  newAttr.bgColor = cw;
  pron::pronSetWindowAttributes(Application::getInstance()->d, this->pronWindow, newAttr, pron::WIN_ATTR_BG_COLOR);*/
}

Widget::~Widget() {
  pron::pronDestroyWindow(Application::getInstance()->d, this->pronWindow);
  Application::getInstance()->widgets.erase(this->pronWindow);
}

void Widget::init() {
  this->parent     = NULL;
  this->eventMask  = 0;
  this->lastX      = -1;
  this->lastY      = -1;
  this->lastWidth  = -1;
  this->lastHeight = -1;
  this->pronWindow = (pron::Window)-1;
}

bool Widget::isPronWindowCreated() {
  return this->pronWindow != (pron::Window)-1;
}

void Widget::subscribeEvent(uint32_t eventMask) {
  this->eventMask |= PRON_EVENTMASK(eventMask);
  if(this->isPronWindowCreated()) {
    //printf("subscribe event for Ox%x\n",this->pronWindow);
    pron::pronSelectInput(Application::getInstance()->d, this->pronWindow,
        this->eventMask);
  }
}

void Widget::unsubscribeEvent(uint32_t eventMask) {
  this->eventMask &= ~PRON_EVENTMASK(eventMask);
  if(this->isPronWindowCreated()) {
    pron::pronSelectInput(Application::getInstance()->d, this->pronWindow,
        this->eventMask);
  }
}

void Widget::dontPropagateEvent(uint32_t eventMask) {
    this->dontPropagateEventMask |= PRON_EVENTMASK(eventMask);
    if(this->isPronWindowCreated()) {
      pron::pronDontPropagateEvent(Application::getInstance()->d, this->pronWindow,
              this->dontPropagateEventMask);
    }
}

void Widget::propagateEvent(uint32_t eventMask) {
  this->dontPropagateEventMask &= ~PRON_EVENTMASK(eventMask);
  if(this->isPronWindowCreated()) {
    pron::pronDontPropagateEvent(Application::getInstance()->d, this->pronWindow,
        this->dontPropagateEventMask);
  }
}

void Widget::setParent(Widget *parent) {
  // TODO à réfléchir si on supprime si ça vaut pas null etc.
  if (this->parent == NULL) {
    this->parent = parent;
    // Creates the window
    this->pronWindow = pron::pronCreateWindow(Application::getInstance()->d,
        this->parent->pronWindow,
        this->x, this->y,
        this->width, this->height);
    //printf("SET PARENT : Ox%x for Ox%x\n", parent->pronWindow, this->pronWindow);
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

Widget* Widget::getParent() {
  return this->parent;
}

void Widget::clear() {
  pron::pronClearWindow(Application::getInstance()->d, this->pronWindow);
}

void Widget::handleMouseDown(MouseButton button __attribute__((unused)), int x __attribute__((unused)), int y __attribute__((unused))) {

}

void Widget::handleMouseReleased(MouseButton button __attribute__((unused)), int x __attribute__((unused)), int y __attribute__((unused))) {

}

void Widget::handleEventWindowCreated() {
}

void Widget::handleEventExpose() {
  //printf("handleEventExpose\n");
  this->draw();
}

void Widget::handleEventPointerMoved(pron::EventPointerMoved *mousePointerEvent) {
  this->mouseMoved(mousePointerEvent->xMove, mousePointerEvent->yMove,
      mousePointerEvent->x, mousePointerEvent->y);
}

void Widget::handleEventMouseButton(pron::EventMouseButton *e) {
  printf("handleEventMouseButton\n");
  if(this->oldButtonsState.b1 != e->b1) {
    if(e->b1) {
      this->handleMouseDown(leftButton, e->x, e->y);
    } else {
      this->handleMouseReleased(leftButton, e->x, e->y);
    }
  }
  if(this->oldButtonsState.b2 != e->b2) {
    if(e->b2) {
      this->handleMouseDown(middleButton, e->x, e->y);
    } else {
      this->handleMouseReleased(middleButton, e->x, e->y);
    }
  }
  if(this->oldButtonsState.b3 != e->b3) {
    if(e->b3) {
      this->handleMouseDown(rightButton, e->x, e->y);
    } else {
      this->handleMouseReleased(rightButton, e->x, e->y);
    }
  }
  this->oldButtonsState = *e;
}

void Widget::handleEventKeyPressed(pron::EventKeyPressed *e __attribute__((unused))) {
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

void Widget::draw() {}

void Widget::setFGColor(const Color &c) {
  pron::pronFreeGC(Application::getInstance()->d, this->fgColor);
  pron::PronGCValues values;
  values.bg = Color(0, 0, 0);
  values.fg = c;
  this->fgColor = pron::pronCreateGC(Application::getInstance()->d, values, pron::GC_VAL_FG | pron::GC_VAL_BG);
}

void Widget::setBGColor(const Color &c) {
  pron::pronFreeGC(Application::getInstance()->d, this->bgColor);
  pron::PronGCValues values;
  values.bg = c;
  values.fg = Color(0, 0, 0);
  this->bgColor = pron::pronCreateGC(Application::getInstance()->d, values, pron::GC_VAL_FG | pron::GC_VAL_BG);
}

} // namespace sombrero
