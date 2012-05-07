#include "sombrero.h"
#include "grid.h"
#include "unistd.h"
#include <cstdio>
#include <cstdlib>

int main() {
  sombrero::Application::getInstance()->init();
  
  // Window button

  sombrero::Window w(0, 0, 300, 100);
  char title1[]= "Label 1";
  char title2[] = "Label 2";
  sombrero::Grid g(&w);
  sombrero::Label l1(&g, title1);

  g.newLine();

  sombrero::Label l2(&g, title2);  


  sombrero::Application::getInstance()->sombrerun();

  return 0;
}
