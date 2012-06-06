#ifndef __MIRAR_H__
#define __MIRAR_H__

#include <sombrero.h>
#include <string.h>
#include <stdlib.h>

#define MAIN_WINDOW_WIDTH 640
#define MAIN_WINDOW_HEIGHT 480

class VentanaMirar;

class Mirar :public has_slots<> {
  private:
    VentanaMirar *ventana; /**< la ventana sombrero que contene la aplicacion */
    sombrero::Grid *g;
    sombrero::Button *siguiente;
    sombrero::Button *anterior;
    sombrero::Image *image;
    sombrero::VScrollPane *sp;
    std::vector<std::string> jpegArchivos; /**< los jpeg archivos que la aplicacion debe MIRAR */
    int corrienteArchivo;
    bool ctrlDown;
    
    void verNuevo (bool siguiente);

  public:
    Mirar(std::string camino);
    ~Mirar();

    void inicializacionSombrero();
    void verSiguiente();
    void verAnterior();
    void verInverso();
    void verPoderDeLaFunk();
    void girarImage(bool reloj);
    void correr();

    bool getCtrlDown();
    void setCtrlDown(bool down);
};
  

#endif // __MIRAR_H__
