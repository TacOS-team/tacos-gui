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
      this->aplicacion = aplicacion;
    }

    void handleEventKeyPressed(pron::EventKeyPressed *e) {
      if (e->keysym == pron::PRONK_RIGHT) {
        this->aplicacion->verSiguiente();
      } else if (e->keysym == pron::PRONK_LEFT) {
        this->aplicacion->verAnterior();
      } else if (e->keysym == pron::PRONK_SPACE) {
        this->aplicacion->verInverso();
      }
    }
};

class BotonSiguiente : public sombrero::Button {
  private:
    Mirar * aplicacion;
  protected:
    void handleClick(int x, int y) {
      sombrero::Button::handleClick(x, y);
      //if (button == sombrero::leftButton) {
        this->aplicacion->verSiguiente();
      //}
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

class BotonInvertir : public sombrero::Button {
  private:
    Mirar * aplicacion;
  protected:
    void handleMouseDown(sombrero::MouseButton button, int x, int y) {
      sombrero::Button::handleMouseDown(button, x, y);
      if (button == sombrero::leftButton) {
        this->aplicacion->verInverso();
      }
    }
    
  public:
    BotonInvertir(const char *text, Mirar *aplicacion) : sombrero::Button(text) {
      this->aplicacion = aplicacion;
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
  this->siguiente = new BotonSiguiente("Siguiente",this);
  this->anterior = new sombrero::Button("Anterior");
  this->anterior->clicked.connect(this, &Mirar::verAnterior);
  this->invertir = new BotonInvertir("Invertir",this);
  this->image = new sombrero::Image(this->jpegArchivos[this->corrienteArchivo]);
  this->image->setXOffset((this->sp->getWidth() - this->image->getImageWidth()) / 2);
  this->sp = new sombrero::VScrollPane(this->image);

  ventana->add(g);

  g->attach(anterior,0,0,1,1);
  g->attachNextTo(this->siguiente,this->anterior,sombrero::POS_RIGHT,1,1);
  g->attachNextTo(this->invertir,this->siguiente,sombrero::POS_RIGHT,1,1);
  this->g->newLine();
  this->g->attach(this->sp,0,1,3,8);
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
  this->image->setXOffset((this->sp->getWidth() - this->image->getImageWidth()) / 2);
  this->sp->add(this->image);
  this->g->attach(this->sp,0,1,3,8);
  this->ventana->draw();
}


void Mirar::verSiguiente() {
  this->verNuevo(true);
}

void Mirar::verAnterior() {
  this->verNuevo(false);
}

void Mirar::verInverso() {
  this->image->reverseColors();
  this->image->draw();
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
