#include "checkbox.h"
#include "application.h"

namespace sombrero {

  Checkbox::Checkbox(Container *parent) : Widget(parent) {
    this->state = false; 
    // sign up to EV_MOUSE_BUTTON
    pron::pronSelectInput(Application::getInstance()->d,this->pronWindow,PRON_EVENTMASK(pron::EV_EXPOSE) | PRON_EVENTMASK(pron::EV_MOUSE_BUTTON));
    this->gcValues.fg = Color(255, 0, 0);
    this->gc = pronCreateGC(Application::getInstance()->d,this->gcValues, pron::GC_VAL_FG);
  }

  Checkbox::~Checkbox() {}

  void Checkbox::handleEventMouseButton(pron::PronEvent *e) {
    if ( ((pron::EventMouseButton *) e)->b1) {
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
