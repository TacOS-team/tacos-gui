#include "sombrero.h"
#include "unistd.h"

int main() {
  sombrero::Application::getInstance()->init();
  sombrero::Window w(0, 0, 50, 50);
  sombrero::Canvas c(&w);
  sombrero::Application::getInstance()->sombrerun();
  return 0;
}
