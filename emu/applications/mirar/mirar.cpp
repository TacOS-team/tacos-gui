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
    VentanaMirar(std::string title, int x, int y, int width, int height, Mirar *aplicacion)
        : sombrero::Window(title, x, y, width, height) {
      this->subscribeEvent(pron::EV_KEY_PRESSED);
      this->subscribeEvent(pron::EV_KEY_RELEASED);
      this->aplicacion = aplicacion;
    }

    void handleEventKeyPressed(pron::EventKeyPressed *e) {
      if (e->keysym == pron::PRONK_LCTRL) {
        this->aplicacion->setCtrlDown(true);
      } else if (e->keysym == pron::PRONK_RIGHT) {
        if (this->aplicacion->getCtrlDown()) {
          this->aplicacion->girarImage(true);
        } else {
          this->aplicacion->verSiguiente();
        }
      } else if (e->keysym == pron::PRONK_LEFT) {
        if(this->aplicacion->getCtrlDown()) {
          this->aplicacion->girarImage(false);
        }
        else {
          this->aplicacion->verAnterior();
        }
      } else if (e->keysym == pron::PRONK_UP) {
        this->aplicacion->verInverso();
      } else if (e->keysym == pron::PRONK_DOWN) {
        this->aplicacion->verPoderDeLaFunk();
      }
    }
    void handleEventKeyReleased(pron::EventKeyReleased *e) {
      if (e->keysym == pron::PRONK_LCTRL) {
        this->aplicacion->setCtrlDown(false);
      }
    }
};

Mirar::Mirar(std::string camino) {
  sombrero::Directory *carpeta = new sombrero::Directory (camino);
  std::list<sombrero::FileInfo> archivos = carpeta->entryInfoList();
  std::string file ;

  printf("Puedo ver los archivos siguientes :\n");
  for (std::list<sombrero::FileInfo>::iterator it = archivos.begin(); it != archivos.end(); it++) {
    file = it->getAbsolutePath();
    if ((int)file.find(".jpg") != -1) {
      printf("%s\n",file.c_str());
      this->jpegArchivos.push_back(file);
    }
  }

  this->corrienteArchivo = 0;
  this->ctrlDown = false;

  if (this->jpegArchivos.size() > 0) {
    this->inicializacionSombrero();
  } else {
    printf("Noy hay ninguna archivo jpeg hombre !\n");
    exit(1);
  }
}

void Mirar::inicializacionSombrero() {
  sombrero::Application::getInstance()->init();
  this->ventana = new VentanaMirar("Mirar", 0, 0, MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGHT, this);
  this->g = new sombrero::Grid();
  this->siguiente = new sombrero::Button("Siguiente");
  this->anterior = new sombrero::Button("Anterior");
  this->image = new sombrero::Image(this->jpegArchivos[this->corrienteArchivo]);
  this->sp = new sombrero::VScrollPane(this->image);


  this->anterior->clicked.connect(this, &Mirar::verAnterior);
  this->siguiente->clicked.connect(this, &Mirar::verSiguiente);

  ventana->add(g);
  g->attach(anterior,0,0,1,1);
  g->attachNextTo(this->siguiente,this->anterior,sombrero::POS_RIGHT,1,1);
  this->g->newLine();
  this->g->attach(this->sp,0,1,2,15);
  this->ventana->draw();
}

Mirar::~Mirar() {
}

void Mirar::verNuevo (bool siguiente) {
  if (siguiente) {
    this->corrienteArchivo = (this->corrienteArchivo + 1) % this->jpegArchivos.size();
  } else {
    this->corrienteArchivo = this->corrienteArchivo - 1;
    if (this->corrienteArchivo < 0) {
      this->corrienteArchivo = this->jpegArchivos.size() - 1;
    }
  }
  this->sp->remove(this->image);
  delete this->image;
  this->image = new sombrero::Image(this->jpegArchivos[this->corrienteArchivo]);
  this->sp->add(this->image);
  this->g->attach(this->sp,0,1,2,15);
  this->ventana->draw();
}


void Mirar::verSiguiente() {
  this->verNuevo(true);
}

void Mirar::verAnterior() {
  this->verNuevo(false);
}

void Mirar::girarImage(bool reloj) {
  this->image->rotate(reloj);
  this->sp->remove(this->image);
  this->sp->add(this->image);
  this->g->attach(this->sp,0,1,2,15);
  this->ventana->draw();
}

void Mirar::verPoderDeLaFunk() {
  this->image->applyNegativeFilter();
  this->image->draw();
}

void Mirar::verInverso() {
  this->image->applyPowerfullnessOfTheFonkFilter();
  this->image->draw();
}
bool Mirar::getCtrlDown() {
  return this->ctrlDown;
}

void Mirar::setCtrlDown(bool down) {
  this->ctrlDown = down;
}

void Mirar::correr() {
  sombrero::Application::getInstance()->sombrerun();
}
  
int main (int argc, char ** argv) {
  std::string camino;
  if (argc > 1) {
    camino = (string)argv[1];
  } else {
    printf("Hombre, no me has dado una carpeta !\nVoy a mirar la carpeta corriente.\n");
    camino = "./";
  }
  Mirar m (camino);
  m.correr();
}
