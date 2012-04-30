#include "sombrero.h"

int main () {

  sombrero::Application::getInstance()->init();
  sombrero::Window w(0,0,300,300);
  sombrero::Checkbox cb(&w);

  sombrero::Application::getInstance()->sombrerun();

  return 0;
}
