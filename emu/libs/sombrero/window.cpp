#include "window.h"
#include "widget.h"
#include "application.h"

namespace sombrero {

  Window::Window(int x, int y, int width, int height) 
    : Bin() {
      this->setX(x);
      this->setY(y);
      this->setWidth(width);
      this->setHeight(height);
      // Creates the window
      this->pronWindow = pron::pronCreateWindow(Application::getInstance()->d, Application::getInstance()->d->rootWindow,
                                                this->getX(), this->getY(), this->getWidth(), this->getHeight());

      // Associates the pron::window to the widget
      Application::getInstance()->widgets[this->pronWindow] = this;
      // Maps the window
      pron::pronMapWindow(Application::getInstance()->d, this->pronWindow);
      // Select events
      this->subscribeEvent(pron::EV_EXPOSE);
      this->subscribeEvent(pron::EV_DESTROY_WINDOW);
      this->subscribeEvent(pron::EV_RESIZE_WINDOW);

      this->layout = NULL;
  }

  Window::~Window() {
  }

  void Window::setLayout(Container *layout) {
    this->layout = layout;
    layout->setWidth(this->getWidth());
    layout->setHeight(this->getHeight());
    layout->setX(this->getX());
    layout->setY(this->getY());
    layout->setParent(this);
    this->add(layout);
  }

  void Window::handleEventResizeWindow(int width, int height) {
    Bin::handleEventResizeWindow(width, height);
    if(this->layout != NULL) {
      this->layout->setWidth(width);
      this->layout->setHeight(height);
      this->layout->update();
    }
  }

  void Window::add(Widget *widget) {
    widget->setX(0);
    widget->setY(0);
    widget->setWidth(this->getWidth());
    widget->setHeight(this->getHeight());
    Bin::add(widget);
  }

} // namespace sombrero
