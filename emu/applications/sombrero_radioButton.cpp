#include "sombrero.h"
#include "radioButton.h"
#include "radioButtonGroup.h"

int main () {
  sombrero::Application::getInstance()->init();

  sombrero::Window w(0,0,300,300);
  sombrero::Window w1(0,0,300,300);
  sombrero::Window w2(0,0,300,300);
  
/*  sombrero::RadioButton b2;
  sombrero::RadioButton b1;
  sombrero::RadioButton b3;
  */
  sombrero::Checkbox cb1;
  sombrero::Checkbox cb2;
  sombrero::Checkbox cb3;

 // sombrero::RadioButtonGroup group;
 // sombrero::Grid grid(&w);
/*
  group.add(&b1);
  group.add(&b2);
  group.add(&b3);
*/
  w.add(&cb1);
  w1.add(&cb2);
  w2.add(&cb3);


/*  grid.add(&cb1);
  grid.newLine();
  grid.add(&cb2);
  grid.newLine();
  grid.add(&cb3);
*/
  sombrero::Application::getInstance()->sombrerun();

  return 0;
}
