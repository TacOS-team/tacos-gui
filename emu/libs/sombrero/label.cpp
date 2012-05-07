#include "label.h"
#include "application.h"
#include "string.h"

namespace sombrero {

  Label::Label(Container *parent, char *text) 
    : Widget(parent), text(text){
    }

  Label::~Label() {}

  void Label::draw() {
    //printf("draw label (x, y, w, h) : %d, %d, %d, %d\n", this->getX(), this->getY(), this->getWidth(), this->getHeight());
    // We can clear the window
    pron::pronClearWindow(Application::getInstance()->d, this->pronWindow);
    // Draws the background 
    pron::pronDrawRect(Application::getInstance()->d, this->pronWindow, Application::getInstance()->d->defaultGC, this->getX(), this->getY(), this->getWidth(), this->getHeight());
    // Draws the text
    pron::pronDrawText(Application::getInstance()->d, this->pronWindow, Application::getInstance()->d->defaultGC, this->getWidth() / 2 - strlen(text)/2*5, this->getHeight() / 2, this->text, strlen(this->text));
  }

  void Label::setText(char *text) {
    this->text = text;
  }

} // namespace sombrero
