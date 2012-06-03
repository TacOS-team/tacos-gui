
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

using namespace sombrero;
using namespace std;

int heightFile = 20;

class Fichier;

Fichier * currentButton = NULL;

class Fichier : public Button {
 public:
  signal1<string> open;
  Fichier(string fileName) : Button(fileName) {
  }

  void handleDoubleClick(int x __attribute__((unused)), int y __attribute__((unused))) {
    //printf("Fichier open %s\n", this->getText().c_str());
    currentButton = this;
    this->open(this->getText());
  }
};

class Panel : public sombrero::Container {
 protected:
  vector<Fichier*> boutons;
 public:
  signal1<string> open;

  void openSlot(std::string fileName) {
    //printf("Panel open %s\n", fileName.c_str());
    this->open(fileName);
  }

  void add(Widget *widget) {}
  void remove(Widget *widget) {}

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

  void execUpdate() {
    if(this->getParent()) {
      if((int)boutons.size() * heightFile > this->getParent()->getHeight()) {
        this->setHeight(boutons.size() * heightFile);
      } else {
        this->setHeight(this->getParent()->getHeight());
      }

      Container::execUpdate();
      
      for(size_t i = 0; i < boutons.size(); ++i) {
        boutons[i]->setY(i * heightFile);
        boutons[i]->setX(0);
        boutons[i]->setWidth(this->getWidth());
        boutons[i]->setHeight(heightFile);
        boutons[i]->update();
      }
    }
  }

  std::vector<Widget*> getChildren() {
    std::vector<Widget*> res;
    for(size_t i = 0; i < boutons.size(); ++i) {
      res.push_back(boutons[i]);
    }
    return res;
  }

  void draw() {
    //printf("Panel::draw\n");
    //this->clear();
    if((int)boutons.size() * heightFile < this->getHeight()) {
      pronFillRectangle(Application::getInstance()->d, this->pronWindow,
               this->bgGC,
               0, boutons.size() * heightFile, this->getWidth(),
               this->getHeight()-boutons.size() * heightFile);
    }
  }

};

class MyWindow : public Window {
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
    p->setFiles(d.entryInfoList());
    scrollpane = new ScrollPane(p);
    g->attachNextTo(scrollpane, l, POS_BOTTOM, 1, 6);
    //scrollpane->update();
    g->update();
    p->open.connect(this, &MyWindow::openSlot);
  }

};

int main() {
  MyWindow w("Velo", 50, 50, 300, 100);
  sombrero::Application::getInstance()->sombrerun();

  return 0;
}
