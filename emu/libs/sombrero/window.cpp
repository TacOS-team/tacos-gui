#include "window.h"
#include "widget.h"
#include "application.h"

namespace sombrero {

Window::Window(int x, int y, int width, int height) 
  : Bin(NULL) {
  this->setX(x);
  this->setY(y);
  this->setWidth(width);
  this->setHeight(height);
  // Creates the window
  this->topWindow = pron::pronCreateWindow(Application::getInstance()->d, Application::getInstance()->d->rootWindow, this->getX(), this->getY(), this->getWidth(), this->getHeight());
  // Maps the window
  pron::pronMapWindow(Application::getInstance()->d, this->topWindow);
  // Select events
  pron::pronSelectInput(Application::getInstance()->d, this->topWindow, PRON_EVENTMASK(pron::EV_EXPOSE) | PRON_EVENTMASK(pron::EV_DESTROY_WINDOW));
}

Window::~Window() {
  
}

void Window::add(Widget *widget) {
  widget->setWidth(this->getWidth());
  widget->setHeight(this->getHeight());
  Bin::add(widget);
}

} // namespace sombrero

