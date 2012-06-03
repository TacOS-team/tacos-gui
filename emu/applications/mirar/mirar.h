#ifndef __MIRAR_H__
#define __MIRAR_H__

#include <sombrero.h>
#include <scrollpane.h>
#include <string.h>
#include <stdlib.h>

#define MAIN_WINDOW_WIDTH 640
#define MAIN_WINDOW_HEIGHT 480

class BotonSiguiente;
class BotonAnterior;
class BotonInvertir;
class VentanaMirar;

class Mirar :public has_slots<> {
  private:
    VentanaMirar *ventana; /**< la ventana sombrero que contene la aplicacion */
    sombrero::Grid *g;
    BotonSiguiente *siguiente;
    sombrero::Button *anterior;
    BotonInvertir *invertir;
    sombrero::Image *image;
    sombrero::ScrollPane *sp;
    std::vector<std::string> jpegArchivos; /**< los jpeg archivos que la aplicacion debe MIRAR */
    int corrienteArchivo;

  public:
    Mirar(std::string camino);
    ~Mirar();

    void inicializacionSombrero();
    void verSiguiente();
    void verAnterior();
    void verInverso();
    void correr();

};
  



#endif // __MIRAR_H__
