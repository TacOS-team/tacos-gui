
#include <fileinfo.h>
#include <button.h>
#include <window.h>
#include <grid.h>
#include <container.h>
#include <scrollpane.h>
#include <directory.h>
#include <application.h>
#include <label.h>
#include <cstdio>

using namespace sombrero;
using namespace std;

int heightFile = 20;

class Fichier;

Fichier * currentButton = NULL;

class Fichier : public Button, public has_slots<> {
 public:
  signal1<string> open;
  Fichier(string fileName) : Button(fileName) {
    this->clicked.connect(this, &Fichier::openSlot);
  }

  void openSlot() {
    //printf("Fichier open %s\n", this->getText().c_str());
    currentButton = this;
    this->open(this->getText());
  }
};

class Panel : public sombrero::Container, public has_slots<> {
 protected:
  vector<Fichier*> boutons;
 public:
  signal1<string> open;

  void openSlot(std::string fileName) {
    //printf("Panel open %s\n", fileName.c_str());
    this->open(fileName);
  }

  void setFiles(list<FileInfo> fileList) {
    //printf("Panel::setFiles\n");
    for(size_t i = 0; i < boutons.size(); ++i) {
      if(boutons[i] != currentButton){
        delete boutons[i];
      }
    }
    boutons.clear();
    Fichier * newF;
    if(currentButton == NULL) {
      newF = new Fichier("..");
      newF->open.connect(this, &Panel::openSlot);
      newF->setParent(this);
    } else {
      newF = currentButton;
      newF->setText("..");
    }
    boutons.push_back(newF);
    for(list<FileInfo>::iterator it = fileList.begin(); it != fileList.end(); ++it) {
      newF = new Fichier(it->getFileName());
      boutons.push_back(newF);
      newF->setParent(this);
      newF->open.connect(this, &Panel::openSlot);
    }
    //printf("Panel::setFiles avant update\n");
    this->update();
  }

  void update() {
    //printf("Panel::update debut\n");
    this->clear();
    this->setHeight(boutons.size() * heightFile);
    sombrero::Container::update();
    //printf("Panel::update avant boucle\n");
    for(size_t i = 0; i < boutons.size(); ++i) {
      boutons[i]->setY(i * heightFile);
      boutons[i]->setX(0);
      boutons[i]->setWidth(this->getWidth());
      boutons[i]->setHeight(heightFile);
      boutons[i]->update();
    }
    //printf("Panel::update fin\n");
  }

  void draw() {
    sombrero::Container::draw();
  }
};

class MyWindow : public Window, public has_slots<> {
 protected:
  Grid g;
  Directory d;
  ScrollPane scrollpane;
  Panel p;
  Label l;

 public:

  void openSlot(std::string fileName) {
    //printf("MyWindow open %s\n", fileName.c_str());
    Directory d2(d.getInformations().getAbsolutePath()+"/"+fileName);
    if(d2.getInformations().isDir()) {
      d = d2;
      p.setFiles(d.entryInfoList());
      l.setText(d.getInformations().getAbsolutePath());
    }
  }

  MyWindow(int x, int y, int width, int height)
    : sombrero::Window(x, y, width, height),
      g(this),
      d("."),
      scrollpane(&p),
      l("") {
    g.add(&l);
    l.setText(d.getInformations().getAbsolutePath());
    g.attachNextTo(&scrollpane, &l, POS_BOTTOM, 1, 6);
    g.setWidth(width);
    g.setHeight(height);
    g.setX(0);
    g.setY(0);
    g.update();
    p.setFiles(d.entryInfoList());
    p.open.connect(this, &MyWindow::openSlot);
  }

};

int main() {
  /*Directory d("/");
  list<FileInfo> files = d.entryInfoList();
  for(list<FileInfo>::iterator it = files.begin(); it != files.end(); ++it) {
    if(it->isDir())  printf("D ");
    if(it->isFile()) printf("F ");
    printf("%s\n", it->getFileName().c_str());
  }*/
  MyWindow w(50, 50, 300, 100);
  sombrero::Application::getInstance()->sombrerun();

  return 0;
}