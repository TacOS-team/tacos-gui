#include "window.h"
#include "widget.h"
#include "application.h"

namespace sombrero {

void Window::init(std::string title, int x, int y,
    int width, int height, bool decorate) {
  this->setX(x);
  this->setY(y);
  this->setWidth(width);
  this->setHeight(height);
  // Creates the window
  pron::PronWindowAttributes attr;
  attr.x = this->getX();
  attr.y = this->getY();
  attr.width = this->getWidth();
  attr.height = this->getHeight();
  attr.wm_decorate = decorate;
  strncpy(attr.wm_title, title.c_str(), WM_TITLE_MAX_LEN);
  this->pronWindow = pron::pronCreateWindowExtended(
      Application::getInstance()->d,
      Application::getInstance()->d->rootWindow,
      attr, pron::WIN_ATTR_X | pron::WIN_ATTR_Y | pron::WIN_ATTR_WIDTH |
      pron::WIN_ATTR_HEIGHT | pron::WIN_ATTR_WM_DECORATE |
      pron::WIN_ATTR_WM_TITLE);

  // Associates the pron::window to the widget
  Application::getInstance()->widgets[this->pronWindow] = this;
  // Maps the window
  pron::pronMapWindow(Application::getInstance()->d, this->pronWindow);
  // Select events
  this->subscribeEvent(pron::EV_EXPOSE);
  this->subscribeEvent(pron::EV_DESTROY_WINDOW);
  this->subscribeEvent(pron::EV_RESIZE_WINDOW);
}

Window::Window(std::string title, int x, int y,
    int width, int height, bool decorate)
    : Bin() {
  init(title, x, y, width, height, decorate);
}

Window::~Window() {}

void Window::handleEventResizeWindow(int width, int height) {
  Bin::handleEventResizeWindow(width, height);
  Widget *w = this->getWidget();
  if(w != NULL) {
    w->setWidth(width);
    w->setHeight(height);
    w->update();
  }
}

void Window::add(Widget *widget) {
  widget->setX(0);
  widget->setY(0);
  widget->setWidth(this->getWidth());
  widget->setHeight(this->getHeight());
  Bin::add(widget);
  widget->update();
}

} // namespace sombrero
