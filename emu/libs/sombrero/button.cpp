#include "button.h"
#include "application.h"
#include "string.h"

namespace sombrero {

  Button::Button() : text("") {
    this->init();
  }

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
    this->subscribeEvent(pron::EV_EXPOSE);
    this->subscribeEvent(pron::EV_MOUSE_BUTTON);
  }

  void Button::draw() {
    /*printf("draw button (x, y, w, h) : %d, %d, %d, %d\n", this->getX(), this->getY(),
     this->getWidth(), this->getHeight());*/
    // We can clear the window
    pron::pronClearWindow(Application::getInstance()->d, this->pronWindow);
    // Draws the background 
    pron::pronDrawRect(Application::getInstance()->d, this->pronWindow, Application::getInstance()->d->defaultGC, this->getX(), this->getY(), this->getWidth(), this->getHeight());
    // Draws the text
    pron::pronDrawText(Application::getInstance()->d, this->pronWindow, Application::getInstance()->d->defaultGC, this->getWidth() / 2 - this->text.length()/2 * this->fontSize/2,
      this->getHeight() / 2 + this->fontSize/2, this->text.c_str(), this->text.length());
  }

  void Button::handleEventMouseButton(pron::EventMouseButton *e) {
    //printf("handleEventMouseButton\n");
    // TODO sauvegarder état bouton
    if(e->b1) {
      this->clicked();
    } else {
      this->released();
    }
  }

} // namespace sombrero
