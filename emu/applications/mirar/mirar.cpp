#include <string.h>
#include <sombrero.h>
#include <directory.h>
#include <fileinfo.h>
#include <stdio.h>

#include "widget.h"
#include "mirar.h"

class VentanaMirar : public sombrero::Window {
  private:
    Mirar * aplicacion;
  public:
    VentanaMirar (int x, int y, int width, int height, Mirar *aplicacion) : sombrero::Window(x, y, width, height) {
      this->subscribeEvent(pron::EV_KEY_PRESSED);
      this->aplicacion = aplicacion;
    }

    void handleEventKeyPressed(pron::EventKeyPressed *e) {
      if (e->keysym == pron::PRONK_RIGHT) {
        this->aplicacion->verSiguiente();
      } else if (e->keysym == pron::PRONK_LEFT) {
        this->aplicacion->verAnterior();
      } 
    }
};

class BotonSiguiente : public sombrero::Button {
  private:
    Mirar * aplicacion;
  protected:
    void handleMouseDown(sombrero::MouseButton button, int x, int y) {
      sombrero::Button::handleMouseDown(button, x, y);
      if (button == sombrero::leftButton) {
        this->aplicacion->verSiguiente();
      }
    }
    
  public:
    BotonSiguiente(const char *text, Mirar *aplicacion) : sombrero::Button(text) {
      this->aplicacion = aplicacion;
    }
};
      
class BotonAnterior : public sombrero::Button {
  private:
    Mirar * aplicacion;
  protected:
    void handleMouseDown(sombrero::MouseButton button, int x, int y) {
      sombrero::Button::handleMouseDown(button, x, y);
      if (button == sombrero::leftButton) {
        this->aplicacion->verAnterior();
      }
    }
    
  public:
    BotonAnterior(const char *text, Mirar *aplicacion) : sombrero::Button(text) {
      this->aplicacion = aplicacion;
    }
};


Mirar::Mirar(char * camino) {
  sombrero::Directory *carpeta = new sombrero::Directory ((string &) camino);
  std::list<sombrero::FileInfo> archivos = carpeta->entryInfoList();
  std::string file ;
  file = "a";

  printf("Puedo ver los archivos siguientes :\n");
  for (std::list<sombrero::FileInfo>::iterator it = archivos.begin(); it != archivos.end(); it++) {
    file = it->getAbsolutePath();
    if ((int)file.find(".jpg") != -1) {
      printf("%s\n",file.c_str());
      this->jpegArchivos.push_back(file);
    }
  }

  this->corrienteArchivo = 0;

  if (this->jpegArchivos.size() > 0) {
    this->inicializacionSombrero();
  } else {
    printf("Noy hay ninguna archivo jpeg hombre !\n");
  }

}

void Mirar::inicializacionSombrero() {
  sombrero::Application::getInstance()->init();
  this->ventana = new VentanaMirar (0,0,MAIN_WINDOW_WIDTH,MAIN_WINDOW_HEIGHT, this);
  this->g = new sombrero::Grid();
  this->siguiente = new BotonSiguiente("Siguiente",this);
  this->anterior = new BotonAnterior("Anterior",this);
  this->image = new sombrero::Image(this->jpegArchivos[this->corrienteArchivo]);
  this->sp = new sombrero::ScrollPane(this->image);

  ventana->add(g);

  g->attach(anterior,0,0,1,1);
  g->attachNextTo(siguiente,anterior,sombrero::POS_RIGHT,1,1);
  this->g->newLine();
  this->g->attach(this->sp,0,1,2,8);
}

Mirar::~Mirar() {
}

void Mirar::verSiguiente() {
  this->corrienteArchivo = (this->corrienteArchivo + 1) % this->jpegArchivos.size();
  this->sp->remove(this->image);
  delete this->image;
  this->image = new sombrero::Image(this->jpegArchivos[this->corrienteArchivo]);
  this->sp->add(this->image);
  this->g->attach(this->sp,0,1,2,8);
  this->ventana->draw();
}

void Mirar::verAnterior() {
  this->corrienteArchivo = this->corrienteArchivo - 1;
  if (this->corrienteArchivo < 0) {
    this->corrienteArchivo = this->jpegArchivos.size() - 1;
  }
  this->sp->remove(this->image);
  delete this->image;
  this->image = new sombrero::Image(this->jpegArchivos[this->corrienteArchivo]);
  this->sp->add(this->image);
  this->g->attach(this->sp,0,1,2,8);
  this->ventana->draw();
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
  printf("Noy hay ninguna archivo jpeg hombre !\n");
}
