#include "button.h"
#include "application.h"
#include "string.h"

namespace sombrero {

Button::Button(Container *parent, char *text) 
    : Widget(parent), text(text){
  // Select more events
  pron::pronSelectInput(Application::getInstance()->d, this->topWindow, PRON_EVENTMASK(pron::EV_EXPOSE) | PRON_EVENTMASK(pron::EV_MOUSE_BUTTON));
}

Button::~Button() {}

void Button::draw() {
  // Draws the background 
  pron::pronDrawRect(Application::getInstance()->d, this->topWindow, Application::getInstance()->d->defaultGC, this->getX(), this->getY(), this->getWidth(), this->getHeight());
  // Draws the text
  pron::pronDrawText(Application::getInstance()->d, this->topWindow, Application::getInstance()->d->defaultGC, this->getWidth() / 2, this->getHeight() / 2, this->text, strlen(this->text));
}

} // namespace sombrero
