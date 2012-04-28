#include "button.h"

namespace sombrero {

Button::Button(Container *parent, char *text) 
    : Widget(parent), text(text){
}

Button::~Button() {
}

void Button::draw() {
}

} // namespace sombrero
