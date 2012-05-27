#include <string.h>
#include <sombrero.h>
#include <directory.h>
#include "stdio.h"

#include "mirar.h"

Mirar::Mirar(char * camino) {
  sombrero::Directory *carpeta = new sombrero::Directory ((string &) camino);
  std::list<std::string> archivos = carpeta->entryList();
  this->inicializacionSombrero();
}

void Mirar::inicializacionSombrero() {
  std::string * archivo = new std::string("plant.jpg");
  sombrero::Application::getInstance()->init();
  this->ventana = new sombrero::Window(0,0,MAIN_WINDOW_WIDTH,MAIN_WINDOW_HEIGHT);
  this->g = new sombrero::Grid();
  this->siguiente = new sombrero::Button("Siguiente");
  this->anterior = new sombrero::Button("Anterior");
  this->image = new sombrero::Image(*archivo);
  this->sp = new sombrero::ScrollPane(this->image);

  ventana->add(g);

  g->attach(anterior,0,0,1,1);
  g->attachNextTo(siguiente,anterior,sombrero::POS_RIGHT,1,1);
  this->g->newLine();
  this->g->attach(this->sp,0,1,2,8);
  this->g->draw();
}

Mirar::~Mirar() {
}

void Mirar::correr() {
  sombrero::Application::getInstance()->sombrerun();
}
  
int main (int argc, char ** argv) {
  char * camino;
  if (argc > 1) {
    camino = argv[1];
  } else {
    printf("Hombre, no me has dado una carpeta !\nVoy a mirar la carpeta corriente.\n");
    camino = (char *)malloc(2 * sizeof(char));
    camino[0] = '.';
    camino[1] = '\0';
  }
  Mirar m (camino);
  m.correr();
}
