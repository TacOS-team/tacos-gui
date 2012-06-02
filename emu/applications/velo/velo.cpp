
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

#include <sys/time.h>
#include <unistd.h>

#include <elf.h>

const int delay = 250;

using namespace sombrero;
using namespace std;

int heightFile = 20;

class Fichier;

Fichier * currentButton = NULL;

class Fichier : public Button, public has_slots<> {
 protected:
  struct timeval lastClick;
 public:
  signal1<string> open;
  Fichier(string fileName) : Button(fileName) {
    this->clicked.connect(this, &Fichier::openSlot);
    gettimeofday(&lastClick, NULL);
  }

  void openSlot() {
    //printf("Fichier open %s\n", this->getText().c_str());

    struct timeval tv;
    gettimeofday(&tv, NULL);


    if(((tv.tv_sec-lastClick.tv_sec)*1000000
        +tv.tv_usec-lastClick.tv_usec) < delay*1000) {
      currentButton = this;
      this->open(this->getText());
    } else {
      lastClick = tv;
    }
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
    if((int)boutons.size() * heightFile > this->getParent()->getHeight()) {
      this->setHeight(boutons.size() * heightFile);
    } else {
      this->setHeight(this->getParent()->getHeight());
    }
    sombrero::Container::update();
    //printf("Panel::update avant boucle\n");
    for(size_t i = 0; i < boutons.size(); ++i) {
      boutons[i]->setY(i * heightFile);
      boutons[i]->setX(0);
      boutons[i]->setWidth(this->getWidth());
      boutons[i]->setHeight(heightFile);
      boutons[i]->update();
    }
    sombrero::Container::update();
    //printf("Panel::update fin\n");
  }

  void draw() {
    this->clear();
  }
};

class MyWindow : public Window, public has_slots<> {
 protected:
  Grid *g;
  Directory d;
  ScrollPane *scrollpane;
  Panel *p;
  Label *l;

 public:

  void draw() {
    g->draw();
    sombrero::Container::draw();
  }

  void openSlot(std::string fileName) {
    //printf("MyWindow open %s\n", fileName.c_str());
    Directory d2(d.getInformations().getAbsolutePath()+"/"+fileName);
    if(d2.getInformations().isDir()) {
      d = d2;
      p->setFiles(d.entryInfoList());
      l->setText(d.getInformations().getAbsolutePath());
    } else if(d2.getInformations().isFile()) {
      /*if(fork() == 0) {
        char * arg [] = { NULL};
        execv(d2.getInformations().getAbsolutePath().c_str(), arg);
        exit(0);
      }*/
      if(exec_elf((char*)d2.getInformations().getAbsolutePath().c_str(), 0) < 0) {
        printf("commande invalide.\n");
      }
    }
  }

  MyWindow(std::string title, int x, int y, int width, int height)
    : sombrero::Window(title, x, y, width, height),
      d(".") {
    g = new Grid();
    g->setWidth(width);
    g->setHeight(height);
    g->setX(0);
    g->setY(0);
    this->add(g);
    l = new Label("");
    g->add(l);
    l->setText(d.getInformations().getAbsolutePath());
    p = new Panel();
    scrollpane = new ScrollPane(p);
    g->attachNextTo(scrollpane, l, POS_BOTTOM, 1, 6);
    g->update();
    p->setFiles(d.entryInfoList());
    p->open.connect(this, &MyWindow::openSlot);
  }

};

int main() {
  MyWindow w("Velo", 50, 50, 300, 100);
  sombrero::Application::getInstance()->sombrerun();

  return 0;
}
