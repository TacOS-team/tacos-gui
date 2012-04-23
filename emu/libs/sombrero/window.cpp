#include "window.h"
#include "widget.h"

namespace sombrero {

class Widget;

Window::Window(int x, int y, int width, int height) 
  : Widget(x, y, width, height) {
  
}

Window::~Window() {
  
}

void Window::draw() {
  
}

} // namespace sombrero
