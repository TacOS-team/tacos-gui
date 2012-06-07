#include <string.h>
#include <sombrero.h>
#include <directory.h>
#include <fileinfo.h>
#include <stdio.h>

#include "widget.h"
#include "presentacion.h"

class VentanaPresentacion : public sombrero::Window {
  private:
    Presentacion * aplicacion;
  public:
    VentanaPresentacion(std::string title, int x, int y, int width, int height, Presentacion *aplicacion)
        : sombrero::Window(title, x, y, width, height) {
      this->subscribeEvent(pron::EV_KEY_PRESSED);
      this->aplicacion = aplicacion;
        }

    void handleEventKeyPressed(pron::EventKeyPressed *e) {
      if (e->keysym == pron::PRONK_RIGHT) {
        this->aplicacion->verNuevo(true);
      } else if (e->keysym == pron::PRONK_LEFT) {
        this->aplicacion->verNuevo(false);
      }
    }
};

Presentacion::Presentacion(std::string camino) {
  sombrero::Directory *carpeta = new sombrero::Directory (camino);
  std::vector<sombrero::FileInfo> archivos = carpeta->entryInfoList();
  std::string file ;

  printf("Puedo ver los archivos siguientes :\n");
  for (std::vector<sombrero::FileInfo>::iterator it = archivos.begin(); it != archivos.end(); it++) {
    file = it->getAbsolutePath();
    if (file.find(".jpg") != std::string::npos) {
      printf("%s\n",file.c_str());
      this->jpegArchivos.push_back(file);
    }
  }

  this->corrienteArchivo = 0;

  if (this->jpegArchivos.size() > 0) {
    sort(this->jpegArchivos.begin(), this->jpegArchivos.end());
    this->inicializacionSombrero();
  } else {
    printf("Noy hay ninguna archivo jpeg hombre !\n");
    exit(1);
  }
}

void Presentacion::inicializacionSombrero() {
  this->ventana = new VentanaPresentacion("Presentacion", 0, 0, MAIN_WINDOW_WIDTH, MAIN_WINDOW_HEIGHT, this);
  this->image = new sombrero::Image(this->jpegArchivos[this->corrienteArchivo]);

  this->ventana->add(this->image);
}

Presentacion::~Presentacion() {
}

void Presentacion::verNuevo (bool siguiente) {
  if (siguiente) {
    this->corrienteArchivo = (this->corrienteArchivo + 1) % this->jpegArchivos.size();
  } else {
    this->corrienteArchivo = this->corrienteArchivo - 1;
    if (this->corrienteArchivo < 0) {
      this->corrienteArchivo = this->jpegArchivos.size() - 1;
    }
  }
  this->ventana->remove(this->image);
  delete this->image;
  this->image = new sombrero::Image(this->jpegArchivos[this->corrienteArchivo]);
  this->ventana->add(this->image);
  this->ventana->draw();
}

void Presentacion::correr() {
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
  Presentacion p (camino);
  p.correr();
}
