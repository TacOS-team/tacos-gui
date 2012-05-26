#include <string.h>
#include <sombrero.h>

#include "mirar.h"

Mirar::Mirar(char * path) {
  this->path = path;
  this->initSombrero(path);
}

void Mirar::initSombrero(char * path) {
  sombrero::Application::getInstance()->init();
  this->w = new sombrero::Window(0,0,640,480);
  this->g = new sombrero::Grid();
  this->next = new sombrero::Button("Siguiente");
  this->prev = new sombrero::Button("Anterior");
  this->image = new sombrero::Image((string &)path);
  this->sp = new sombrero::ScrollPane(this->image);

  w->add(g);

  this->g->add(this->next);
  this->g->add(this->prev);
  this->g->newLine();
  this->g->add(this->sp);
  this->g->draw();
}

Mirar::~Mirar() {
}

void Mirar::run() {
  sombrero::Application::getInstance()->sombrerun();
}
  
int main (int argc, char ** argv) {
  char * path;
  if (argc > 1) {
    path = argv[1];
  } else {
    path = (char *)malloc(2 * sizeof(char));
    path[0] = '.';
    path[1] = '\0';
  }
  Mirar m (path);
  m.run();
}
