#include "label.h"
#include "application.h"
#include "string.h"

namespace sombrero {

Label::Label(const std::string text) {
  this->text = text;
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
  this->clear();
  // Draws the line 
  pron::pronDrawRect(Application::getInstance()->d, this->pronWindow,
      this->fgGC, 0, 0, this->getWidth(), this->getHeight());
  // Draws the text
  pron::pronDrawText(Application::getInstance()->d, this->pronWindow,
      this->fgGC, this->getWidth() / 2, this->getHeight() / 2,
      this->text.c_str(),this->text.length(), CENTER, MIDDLE);
}

void Label::setText(const char *text) {
  this->text = text;
  this->draw();
}

void Label::setText(const std::string text) {
  this->text = text;
  this->draw();
}

} // namespace sombrero
