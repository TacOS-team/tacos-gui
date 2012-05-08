#include "button.h"
#include "application.h"
#include "string.h"

namespace sombrero {

  Button::Button(const char *text) : text(text) {
    this->init();
  }

  Button::Button(const std::string text) : text(text) {
    this->init();
  }

  void Button::init() {
    this->fontSize = 10;
  }

  Button::~Button() {}

  void Button::setParent(Container *parent) {
    Widget::setParent(parent);
    // Select more events
    pron::pronSelectInput(Application::getInstance()->d, this->pronWindow,
              PRON_EVENTMASK(pron::EV_EXPOSE)
            | PRON_EVENTMASK(pron::EV_MOUSE_BUTTON));
  }

  void Button::draw() {
    // We can clear the window
    pron::pronClearWindow(Application::getInstance()->d, this->pronWindow);
    // Draws the background 
    pron::pronDrawRect(Application::getInstance()->d, this->pronWindow, Application::getInstance()->d->defaultGC, this->getX(), this->getY(), this->getWidth(), this->getHeight());
    // Draws the text
    pron::pronDrawText(Application::getInstance()->d, this->pronWindow, Application::getInstance()->d->defaultGC, this->getWidth() / 2 - this->text.length()/2 * this->fontSize/2,
      this->getHeight() / 2 + this->fontSize/2, this->text.c_str(), this->text.length());
  }

} // namespace sombrero
