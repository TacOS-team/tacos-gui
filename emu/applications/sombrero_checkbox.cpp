#include "sombrero.h"

int main () {

  sombrero::Application::getInstance()->init();
  sombrero::Window w("Test checkbox", 0, 0, 300, 300);
  sombrero::Checkbox cb;
  w.add(&cb);
  

  sombrero::Application::getInstance()->sombrerun();

  return 0;
}
