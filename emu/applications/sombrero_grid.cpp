#include "sombrero.h"
#include "unistd.h"
#include <cstdio>
#include <cstdlib>

sombrero::Label *pLabel;

class MyWindow : public sombrero::Window {
 public:
  MyWindow(std::string title, int x, int y, int width, int height)
    : sombrero::Window(title, x, y, width, height) {

  }
  void changeLabel() {
    pLabel->setText("Oh yeah !!!");
    pLabel->update();
  }
  void resize() {
    pLabel->setHeight(pLabel->getHeight()+20);
    pLabel->update();
    //pLabel->getParent()->update();
    printf("New label size %d\n", pLabel->getHeight());
  }
  void resize2() {
    pLabel->setHeight(pLabel->getHeight()-20);
    pLabel->update();
    printf("New label size %d\n", pLabel->getHeight());
  }

  void newValue(int newVal) {
    printf("New Value slot \\o/ : %d\n", newVal);
  }
};

void grid1() {

  sombrero::Application::getInstance()->init();
  
  // Window button

  MyWindow w("Roxxing grid", 50, 50, 300, 100);
  sombrero::Grid g;

  sombrero::Label labelScrollPane("Oh yeah !!!!");
  pLabel = &labelScrollPane;
  labelScrollPane.setHeight(150);

  sombrero::VScrollPane scrollpane(&labelScrollPane);
  g.attachNextTo(&scrollpane,NULL, sombrero::POS_LEFT,2,1);

  sombrero::Button bResize("Augmenter taille");
  g.newLine();
  g.add(&bResize);
  bResize.clicked.connect(&w, &MyWindow::resize);

  sombrero::Button bResize2("Diminuer taille");
  g.add(&bResize2);
  bResize2.clicked.connect(&w, &MyWindow::resize2);
  
  w.add(&g);

  sombrero::Application::getInstance()->sombrerun();
}

void grid2() {

  sombrero::Application::getInstance()->init();
  
  // Window button

  MyWindow w("Roxxing grid 2", 50, 50, 300, 100);
  sombrero::Grid g;

  sombrero::Grid g2;
  sombrero::Label l1("label1");
  sombrero::Label ssl1("ss-label1");
  sombrero::Label ssl2("ss-label2");

  g.add(&l1);
  g.add(&g2);
  g2.add(&ssl1);
  g2.newLine();
  g2.add(&ssl2);
  
  w.add(&g);

  sombrero::Application::getInstance()->sombrerun();
}

void grid3() {

  sombrero::Application::getInstance()->init();
  
  // Window button

  MyWindow w("Roxxing grid 3", 50, 50, 300, 100);
  sombrero::Grid g;
  sombrero::Image i("../../ressources/images/tacos.jpg");
  sombrero::Image handCursor("../../ressources/cursors/black_hand.bmp");

  sombrero::DoubleScrollPane scrollPane(&i);

  g.add(&scrollPane);
  
  w.add(&g);

  i.setCursor(handCursor.getPixMap());

  sombrero::Application::getInstance()->sombrerun();
}

void gridRemove() {

  sombrero::Application::getInstance()->init();
  
  // Window button

  MyWindow w("Roxxing grid 2", 50, 50, 300, 100);
  sombrero::Grid g;

  sombrero::Label *l1 = new sombrero::Label("label1");
  sombrero::Label *l2 = new sombrero::Label("label2");
  sombrero::Label *ssl1 = new sombrero::Label("ss-label1");
  sombrero::Label *ssl2 = new sombrero::Label("ss-label2");

  g.attachNextTo(l1,NULL, sombrero::POS_LEFT,1,2);
  g.attachNextTo(l2,l1, sombrero::POS_RIGHT,1,2);
  g.attachNextTo(ssl1,l2, sombrero::POS_RIGHT,1,1);
  g.attachNextTo(ssl2,ssl1, sombrero::POS_BOTTOM,1,1);
  
  w.add(&g);

  /*g.removeColumns(1,1);
  delete l2;*/

  /*g.removeColumns(1,2);
  delete l2;
  delete ssl1;
  delete ssl2;*/

  /*g.removeColumns(0,1);
  delete l1;*/

  /*g.removeColumns(0,2);
  delete l2;
  delete l1;*/

  /*g.remove(l1);
  delete l1;*/

  /*g.remove(l2);
  delete l2;*/

  g.remove(l1);
  delete l1;
  g.remove(l2);
  delete l2;

  //g.remove(ssl1);
  //g.remove(ssl2);

  //delete ssl1;
  //delete ssl2;

  /*g.removeRows(0,1);
  delete ssl1;*/

  /*g.removeRows(1,1);
  delete ssl2;*/

  sombrero::Application::getInstance()->sombrerun();
}

int main() {
  gridRemove();
  return 0;
}

