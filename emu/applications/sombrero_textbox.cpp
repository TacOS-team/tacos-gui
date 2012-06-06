#include "sombrero.h"

int main() {
  sombrero::Window w("Test checkbox", 0, 0, 400, 150);
  sombrero::Textbox t;
  w.add(&t);
  sombrero::Application::getInstance()->sombrerun();

  return 0;
}
