#include "checkbox.h"
#include "application.h"

namespace sombrero {

  Checkbox::Checkbox(Container *parent) : Widget(parent) {
    this->state = false; 
    // sign up to EV_MOUSE_BUTTON
    pron::pronSelectInput(Application::getInstance()->d,this->topWindow,PRON_EVENTMASK(pron::EV_EXPOSE) | PRON_EVENTMASK(pron::EV_MOUSE_BUTTON));
    COLOR(this->gcValues.fg, 24).r = 255;
    COLOR(this->gcValues.fg, 24).g = 0;
    COLOR(this->gcValues.fg, 24).b = 0;
    this->gc = pronCreateGC(Application::getInstance()->d,this->gcValues, pron::GC_VAL_FG);
  }

  Checkbox::~Checkbox() {}


  void Checkbox::handleEvent(pron::PronEvent *e) {
    switch (e->type) {
      case pron::EV_EXPOSE: 
        this->draw();
        break;

      case pron::EV_MOUSE_BUTTON: 
        if (((pron::EventMouseButton *)e)->b1) {
          this->clicked(); // If user wants to know the button state has changed
          this->state = !this->state;
          this->draw(); // redraw the window/widget
        }
        break;

      case pron::EV_DESTROY_WINDOW:
        exit(1);
        break;
      default:
        break; 
    }
  }

  bool Checkbox::isChecked() {
    return state;
  }

  void Checkbox::draw() {
    printf("ubeognz\n");
    if (state == true) {
      COLOR(this->gcValues.fg, 24).r = 0;
      COLOR(this->gcValues.fg, 24).g = 255;
    }
    else {
      COLOR(this->gcValues.fg, 24).r = 255;
      COLOR(this->gcValues.fg, 24).g = 0;
    }
    pron::pronChangeGC(Application::getInstance()->d,this->gc,this->gcValues, pron::GC_VAL_FG);
    pron::pronFillRectangle(Application::getInstance()->d,this->topWindow,this->gc, this->getX(), this->getY(),this->getWidth(), this->getHeight());
  }
} // namespace sombrero
