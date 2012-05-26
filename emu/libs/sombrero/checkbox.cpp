#include "checkbox.h"
#include "application.h"

namespace sombrero {

Checkbox::Checkbox() {
  this->state = false; 
  this->gcValues.fg = Color(255, 0, 0);
  
  // sign up to the wanted event
  this->subscribeEvent(pron::EV_EXPOSE);
  this->subscribeEvent(pron::EV_MOUSE_BUTTON);

  this->gc = pronCreateGC(Application::getInstance()->d,this->gcValues, pron::GC_VAL_FG);
}

Checkbox::~Checkbox() {
}

void Checkbox::handleMouseClicked(MouseButton button) {
  if (button == leftButton) {
    this->clicked();
    this->state = !this->state;
    this->draw();
  }
}

bool Checkbox::isChecked() {
  return state;
}

void Checkbox::draw() {
  if (state == true) {
    this->gcValues.fg = Color(0, 255, 0);
  }
  else {
    this->gcValues.fg = Color(255, 0, 0);
  }
  pron::pronChangeGC(Application::getInstance()->d,this->gc,this->gcValues, pron::GC_VAL_FG);
  pron::pronFillRectangle(Application::getInstance()->d,this->pronWindow,this->gc, this->getX(), this->getY(),this->getWidth(), this->getHeight());
}
} // namespace sombrero
