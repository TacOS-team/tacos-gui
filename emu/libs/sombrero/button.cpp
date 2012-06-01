#include "button.h"
#include "application.h"
#include "string.h"

namespace sombrero {

Button::Button() : text("") {
  this->init();
  this->subscribeEvent(pron::EV_EXPOSE);
  this->subscribeEvent(pron::EV_MOUSE_BUTTON);
}

Button::Button(const char *text) : text(text) {
  this->init();
  this->subscribeEvent(pron::EV_EXPOSE);
  this->subscribeEvent(pron::EV_MOUSE_BUTTON);
}

Button::Button(const std::string &text) : text(text) {
  this->init();
  this->subscribeEvent(pron::EV_EXPOSE);
  this->subscribeEvent(pron::EV_MOUSE_BUTTON);
}

void Button::init() {
  // Select more events
  this->subscribeEvent(pron::EV_EXPOSE);
  this->subscribeEvent(pron::EV_MOUSE_BUTTON);
}

Button::~Button() {}

std::string Button::getText() {
  return this->text;
}

void Button::setText(std::string text) {
  this->text = text;
}

void Button::draw() {
  /*printf("draw button (x, y, w, h) : %d, %d, %d, %d\n", this->getX(), this->getY(),
  this->getWidth(), this->getHeight());*/
  // We can clear the window
  this->clear();
  // Draws the line 
  pron::pronDrawRect(Application::getInstance()->d, this->pronWindow,
      Application::getInstance()->d->defaultGC, 0, 0,
      this->getWidth(), this->getHeight());
  // Draws the text
  int textWidth, textHeight;
  pronTextSize(Application::getInstance()->d,
      Application::getInstance()->d->defaultGC,
      this->text.c_str(), this->text.length(), &textWidth, &textHeight);
  pron::pronDrawText(Application::getInstance()->d, this->pronWindow,
      Application::getInstance()->d->defaultGC,
      (this->getWidth() - textWidth) / 2, (this->getHeight() - textHeight) / 2,
      this->text.c_str(), this->text.length());
}

void Button::handleMouseDown(MouseButton button, int x __attribute__((unused)), int y __attribute__((unused))) {
  //printf("handleMouseClick\n");
  if(button == leftButton) {
    this->clicked();
  }
}

void Button::handleMouseReleased(MouseButton button, int x __attribute__((unused)), int y __attribute__((unused))) {
  //printf("handleMouseReleased\n");
  if(button == leftButton) {
    this->released();
  }
}

} // namespace sombrero
