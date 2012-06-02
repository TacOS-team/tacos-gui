#include "sombrero.h"
#include "radioButton.h"
#include "radioButtonGroup.h"
#include "stdio.h"



int main () {
  sombrero::Application::getInstance()->init();

  sombrero::Window w1("Bouton 1", 0, 0, 100, 100);
  sombrero::Window w2("Bouton 2", 0, 0, 100, 100);
  sombrero::Window w3("Bouton 3", 0, 0, 100, 100);
  sombrero::RadioButtonGroup g;
  sombrero::RadioButton rb1;
  sombrero::RadioButton rb2;
  sombrero::RadioButton rb3;

  // rb3 is the only one to true in the end
  rb1.setState(true);
  rb2.setState(true);
  rb3.setState(true);

  // Three ways to associate a group with a radioButton
  rb1.setGroup(&g);
  rb2.joinGroup(&rb1);
  g.add(&rb3);

  w1.add(&rb1);
  w2.add(&rb2);
  w3.add(&rb3);
    

  sombrero::Application::getInstance()->sombrerun();

  return 0;
}
