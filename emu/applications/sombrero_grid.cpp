#include "sombrero.h"
#include "grid.h"
#include "vscrollbar.h"
#include "unistd.h"
#include <cstdio>
#include <cstdlib>

sombrero::Label *pLabel;

class MyWindow : public sombrero::Window, public has_slots<> {
 public:
  MyWindow(int x, int y, int width, int height)
    : sombrero::Window(x, y, width, height) {

  }
  void changeLabel() {
    pLabel->setText("Oh yeah !!!");
    pLabel->update();
  }

  void newValue(int newVal) {
    printf("New Value slot \\o/ : %d\n", newVal);
  }
};

int main() {
  sombrero::Application::getInstance()->init();
  
  // Window button

  MyWindow w(50, 50, 300, 90);
  sombrero::Grid g(&w);
  /*sombrero::Label l1("Label 1");
  pLabel = &l1;

  g.add(&l1);

  sombrero::Label l2("Label 2");
  g.add(&l2);

  //g.newLine();

  sombrero::Label l3("Label 3");
  g.attachNextTo(&l3, &l1,sombrero::POS_TOP,2,1);


  sombrero::Label l4("Label 4");
  g.attachNextTo(&l4, &l1,sombrero::POS_BOTTOM,2,1);


  sombrero::Label l5("Label 5");
  g.attachNextTo(&l5, &l3,sombrero::POS_LEFT,1,3);


  sombrero::Label l6("Label 6");
  g.attach(&l6,0,3,3,1);*/

  /*sombrero::Grid g2;

  g.attachNextTo(&g2,NULL, sombrero::POS_LEFT,1,4);


  sombrero::Label l7("Label 7");
  g2.add(&l7);

  g2.newLine();

  sombrero::Label l8("Label 8");
  g2.add(&l8);

  g2.newLine();

  sombrero::Label l9("Label 9");
  g2.add(&l9);*/

  /** /sombrero::Button b1("Bouton 1");
  g.add(&b1);/**/

  //b1.clicked.connect(&w, &MyWindow::changeLabel);

  sombrero::VScrollBar vsb1;
  g.attachNextTo(&vsb1,NULL, sombrero::POS_LEFT,1,4);
  vsb1.newValue.connect(&w, &MyWindow::newValue);
  //vsb1.setRange(225,99999);

  /*sombrero::Label l10("Label 10");
  g.attachNextTo(&l10,NULL, sombrero::POS_LEFT,1,4);*/


  sombrero::Application::getInstance()->sombrerun();

  return 0;
}
