#ifndef __MIRAR_H__
#define __MIRAR_H__

#include <sombrero.h>
#include <scrollpane.h>

#define MAIN_WINDOW_WIDTH 640
#define MAIN_WINDOW_HEIGHT 480

class Mirar :public has_slots<> {
  private:
    sombrero::Window *ventana; /**< la ventana sombrero que contene la aplicacion */
    sombrero::Grid *g;
    sombrero::Button *siguiente;
    sombrero::Button *anterior;
    sombrero::Image *image;
    sombrero::ScrollPane *sp;
    std::vector<std::string> *jpegArchivos; /**< los jpeg archivos que la aplicacion debe MIRAR */

  public:
    Mirar(char * camino);
    ~Mirar();

    void inicializacionSombrero();
    void correr();

};
  



#endif // __MIRAR_H__
