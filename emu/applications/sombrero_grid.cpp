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

  sombrero::HScrollBar scrollBar;

  g.add(&scrollBar);
  
  w.add(&g);

  sombrero::Application::getInstance()->sombrerun();
}

int main() {
  grid3();
  return 0;
}

