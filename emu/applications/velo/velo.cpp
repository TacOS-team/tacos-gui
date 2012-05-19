
#include <fileinfo.h>
#include <button.h>
#include <window.h>
#include <container.h>
#include <scrollpane.h>
#include <directory.h>
#include <application.h>
#include <cstdio>

using namespace sombrero;
using namespace std;

int heightFile = 20;

class Fichier : public Button, public has_slots<> {
 public:
  signal1<string> open;
  Fichier(string fileName) : Button(fileName) {
    this->clicked.connect(this, &Fichier::openSlot);
  }

  void openSlot() {
    this->open(this->text);
  }
};

class Panel : public sombrero::Container {
 protected:
  vector<Fichier*> boutons;
  list<FileInfo> files;
 public:
  void setFiles(list<FileInfo> fileList) {
    for(size_t i = 0; i < boutons.size(); ++i) {
      delete boutons[i];
    }
    boutons.clear();
    this->files = fileList;
    for(list<FileInfo>::iterator it = fileList.begin(); it != fileList.end(); ++it) {
      Fichier * newF = new Fichier(it->getFileName());
      boutons.push_back(newF);
      newF->setY(boutons.size() * heightFile);
      newF->setX(0);
      newF->setWidth(this->getWidth());
      newF->setHeight(heightFile);
      newF->setParent(this);
    }
  }
  void update() {
    this->setHeight((files.size()+1) * heightFile);
    sombrero::Container::update();
  }
  void draw() {
    for(size_t i = 0; i < boutons.size(); ++i) {
      boutons[i]->draw();
    }
    sombrero::Container::draw();
  }
};

class MyWindow : public Window, public has_slots<> {
 protected:
  Directory d;
  ScrollPane scrollpane;
  Panel p;

 public:
  MyWindow(int x, int y, int width, int height)
    : sombrero::Window(x, y, width, height),
      d("."),
      scrollpane(&p) {
    p.setFiles(d.entryInfoList());
    scrollpane.setWidth(width);
    scrollpane.setHeight(height);
    scrollpane.setX(0);
    scrollpane.setY(0);
    scrollpane.setParent(this);
    scrollpane.update();
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