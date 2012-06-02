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
  this->fgGC = pron::pronCreateGC(Application::getInstance()->d, values, pron::GC_VAL_FG | pron::GC_VAL_BG);
  values.bg = cw;
  values.fg = cb;
  this->bgGC = pron::pronCreateGC(Application::getInstance()->d, values, pron::GC_VAL_FG | pron::GC_VAL_BG);
}

Widget::~Widget() {
  pron::pronDestroyWindow(Application::getInstance()->d, this->pronWindow);
  Application::getInstance()->widgets.erase(this->pronWindow);
}

void Widget::init() {
  this->parent                = NULL;
  this->eventMask             = 0;
  this->lastAttributes.x      = -1;
  this->lastAttributes.y      = -1;
  this->lastAttributes.width  = -1;
  this->lastAttributes.height = -1;
  this->pronWindow            = (pron::Window) -1;
  // Subscribe to the expose event for everyone
  this->subscribeEvent(pron::EV_EXPOSE);
}

bool Widget::isPronWindowCreated() {
  return this->pronWindow != (pron::Window) -1;
}

void Widget::subscribeEvent(uint32_t eventMask) {
  this->eventMask |= PRON_EVENTMASK(eventMask);
  if (this->isPronWindowCreated()) {
    //printf("subscribe event for Ox%x\n",this->pronWindow);
    pron::pronSelectInput(Application::getInstance()->d, this->pronWindow,
        this->eventMask);
  }
}

void Widget::unsubscribeEvent(uint32_t eventMask) {
  this->eventMask &= ~PRON_EVENTMASK(eventMask);
  if (this->isPronWindowCreated()) {
    pron::pronSelectInput(Application::getInstance()->d, this->pronWindow,
        this->eventMask);
  }
}

void Widget::dontPropagateEvent(uint32_t eventMask) {
    this->dontPropagateEventMask |= PRON_EVENTMASK(eventMask);
    if (this->isPronWindowCreated()) {
      pron::pronDontPropagateEvent(Application::getInstance()->d, this->pronWindow,
              this->dontPropagateEventMask);
    }
}

void Widget::propagateEvent(uint32_t eventMask) {
  this->dontPropagateEventMask &= ~PRON_EVENTMASK(eventMask);
  if (this->isPronWindowCreated()) {
    pron::pronDontPropagateEvent(Application::getInstance()->d, this->pronWindow,
        this->dontPropagateEventMask);
  }
}

void Widget::setParent(Widget *parent) {
  // TODO à réfléchir si on supprime si ça vaut pas null etc.
  if (this->parent == NULL && parent->isPronWindowCreated()) {
    this->parent = parent;
    // Creates the window
    this->pronWindow = pron::pronCreateWindow(Application::getInstance()->d,
        this->parent->pronWindow, this->getX(), this->getY(),
        this->getWidth(), this->getHeight());
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
  return this->attributes.width;
}

void Widget::setWidth(int width) {
  this->attributes.width = width;
}

int Widget::getHeight() {
  return this->attributes.height;
}

void Widget::setHeight(int height) {
  this->attributes.height = height;
}

void Widget::updatePronSize() {
  if (this->lastAttributes.width != this->getWidth() ||
      this->lastAttributes.height != this->getHeight()) {
    this->lastAttributes.width  = this->getWidth();
    this->lastAttributes.height = this->getHeight();
    this->resized();
    pronResizeWindow(Application::getInstance()->d, this->pronWindow,
        this->getWidth(), this->getHeight());
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
  return this->attributes.x;
}

void Widget::setX(int x) {
  this->attributes.x = x;
}

int Widget::getY() {
  return this->attributes.y;
}

void Widget::setY(int y) {
  this->attributes.y = y;
}

void Widget::updatePronPosition() {
  if (this->lastAttributes.x != this->getX() ||
      this->lastAttributes.y != this->getY()) {
    this->lastAttributes.x = this->getX();
    this->lastAttributes.y = this->getY();
    pronMoveWindowTo(Application::getInstance()->d, this->pronWindow,
        this->getX(), this->getY());
  }
}

void Widget::updateBGColor() {
  if (this->attributes.bgColor.getRGB24() != this->lastAttributes.bgColor.getRGB24()) {
    pron::PronWindowAttributes newAttr;
    newAttr.bgColor = this->attributes.bgColor;
    pron::pronSetWindowAttributes(Application::getInstance()->d, this->pronWindow, newAttr, pron::WIN_ATTR_BG_COLOR);
  }
}

void Widget::update() {
  this->updatePronPosition();
  this->updatePronSize();
  this->updateBGColor();
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

  this->lastAttributes.height = height;
  this->lastAttributes.width  = width;

  if (isResized) {
    this->setWidth(width);
    this->setHeight(height);
    this->resized();
  }
}

void Widget::draw() {}

void Widget::setFGColor(const Color &c) {
  pron::PronGCValues values;
  values.bg = Color(0, 0, 0);
  values.fg = c;
  pron::pronChangeGC(Application::getInstance()->d, this->fgGC, values,
      pron::GC_VAL_FG | pron::GC_VAL_BG);
}

void Widget::setBGColor(const Color &c) {
  this->attributes.bgColor = c;
  pron::PronGCValues values;
  values.bg = Color(0, 0, 0);
  values.fg = c;
  pron::pronChangeGC(Application::getInstance()->d, this->bgGC, values,
      pron::GC_VAL_FG | pron::GC_VAL_BG);
  // We have to update pron attributes
  this->update();
}

} // namespace sombrero
