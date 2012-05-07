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
  char title3[] = "Label 3";
  sombrero::Grid g(&w);
  sombrero::Label l1(title1);

  g.add(&l1);

  sombrero::Label l2(title2);
  g.add(&l2);

  g.newLine();

  sombrero::Label l3(title3);
  g.add(&l3);


  sombrero::Application::getInstance()->sombrerun();

  return 0;
}
