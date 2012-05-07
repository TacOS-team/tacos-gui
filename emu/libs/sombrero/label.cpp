#include "label.h"
#include "application.h"
#include "string.h"

namespace sombrero {

  Label::Label(Container *parent, const std::string text) 
    : Widget(parent), text(text) {
    this->init();
  }

  Label::Label(const std::string text) {
    this->text = text;
    this->init();
  }

  Label::Label(Container *parent, const char *text) 
    : Widget(parent), text(text) {
    this->init();
  }

  Label::Label(const char *text) {
    this->text = text;
    this->init();
  }

  void Label::init() {
    this->fontSize = 10;
  }

  Label::~Label() {}

  void Label::draw() {
    printf("draw label (x, y, w, h) : %d, %d, %d, %d\n", this->getX(), this->getY(), this->getWidth(), this->getHeight());
    // We can clear the window
    pron::pronClearWindow(Application::getInstance()->d, this->pronWindow);
    // Draws the background 
    pron::pronDrawRect(Application::getInstance()->d, this->pronWindow, Application::getInstance()->d->defaultGC, this->getX(), this->getY(), this->getWidth(), this->getHeight());
    // Draws the text
    pron::pronDrawText(Application::getInstance()->d, this->pronWindow, Application::getInstance()->d->defaultGC, this->getWidth() / 2 - this->text.length()/2*this->fontSize/2,
      this->getHeight() / 2 + this->fontSize/2, this->text.c_str(), this->text.length());
  }

  void Label::setText(const char *text) {
    this->text = text;
  }

  void Label::setText(const std::string text) {
    this->text = text;
  }

} // namespace sombrero
