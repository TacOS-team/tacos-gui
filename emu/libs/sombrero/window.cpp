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
  }

  Window::~Window() {
  }

  void Window::handleEventResizeWindow(int width, int height) {
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
  }

} // namespace sombrero
