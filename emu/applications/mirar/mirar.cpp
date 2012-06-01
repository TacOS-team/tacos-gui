#include <string.h>
#include <sombrero.h>
#include <directory.h>
#include <stdio.h>

#include "widget.h"
#include "mirar.h"


class BotonSiguiente : public sombrero::Button {
  private:
    Mirar * aplicacion;
  protected:
    void handleMouseClicked(sombrero::MouseButton button, int x, int y) {
      sombrero::Button::handleMouseClicked(button, x, y);
      if (button == sombrero::leftButton) {
        printf("CLIC SOBRE EL BUTTON SIGUIENTE\n");
        this->aplicacion->verSiguiente();
      }
    }

  public:
    BotonSiguiente(const char *text, Mirar *aplicacion) : sombrero::Button(text) {
      this->aplicacion = aplicacion;
    }
};
      
class BotonAnterior : public sombrero::Button {
  public:
    BotonAnterior(const char *text) : sombrero::Button(text) {
    }
  protected:
    void handleMouseClicked(sombrero::MouseButton button, int x, int y) {
      sombrero::Button::handleMouseClicked(button, x, y);
      if (button == sombrero::leftButton) {
        printf("CLIC SOBRE EL BUTTON ANTERIOR\n");
      }
    }
};

bool esUnJpegHombre (std::string &nombre) {
  bool ret = false;
  if (nombre.size() > 4 && nombre.find_last_of(".jpeg") != nombre.npos) {
    ret = true;
  }
  return ret;
}
  

Mirar::Mirar(char * camino) {
  /*sombrero::Directory *carpeta = new sombrero::Directory ((string &) camino);
  std::list<std::string> archivos = carpeta->entryList();
  for (std::list<std::string>::iterator it = archivos.begin(); it != archivos.end(); it++) {
    if (esUnJpegHombre((string &)it)) {
      this->jpegArchivos.push_back((string &)it);
    }
  }
  */

  this->corrienteArchivo = 0;
  this->jpegArchivos.push_back("/home/garfunk/Bureau/madre.jpg");
  this->jpegArchivos.push_back("/home/garfunk/Bureau/chucknorris.jpg");
  this->inicializacionSombrero();
}

void Mirar::inicializacionSombrero() {
  sombrero::Application::getInstance()->init();
  this->ventana = new sombrero::Window(0,0,MAIN_WINDOW_WIDTH,MAIN_WINDOW_HEIGHT);
  this->g = new sombrero::Grid();
  this->siguiente = new BotonSiguiente("Siguiente",this);
  this->anterior = new BotonAnterior("Anterior");
  this->image = new sombrero::Image(this->jpegArchivos[this->corrienteArchivo]);
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

void Mirar::verSiguiente() {
  this->corrienteArchivo = (this->corrienteArchivo + 1) % this->jpegArchivos.size();
  this->image = new sombrero::Image(this->jpegArchivos[this->corrienteArchivo]); // @TODO cambiar eso
  this->sp = new sombrero::ScrollPane(this->image);
  this->g->attach(this->sp,0,1,2,8);
  this->g->draw();
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
