#include "sombrero.h"
#include "grid.h"
#include "unistd.h"
#include <cstdio>
#include <cstdlib>

int main() {
  sombrero::Application::getInstance()->init();
  
  // Window button

  sombrero::Window w(0, 0, 300, 90);
  sombrero::Grid g(&w);
  sombrero::Label l1("Label 1");

  g.add(&l1);

  sombrero::Label l2("Label 2");
  g.add(&l2);

  //g.newLine();

  sombrero::Label l3("Label 3");
  g.attachNextTo(&l3, &l2,sombrero::POS_RIGHT,3,2);


  sombrero::Button b1("bouton 1");
  g.attachNextTo(&b1, &l1,sombrero::POS_BOTTOM,2,1);


  sombrero::Application::getInstance()->sombrerun();

  return 0;
}
