#ifndef __PRESENTACION_H__
#define __PRESENTACION_H__

#include <sombrero.h>
#include <string.h>
#include <stdlib.h>

#define MAIN_WINDOW_WIDTH 640
#define MAIN_WINDOW_HEIGHT 480

class VentanaPresentacion;

class Presentacion :public has_slots<> {
  private:
    VentanaPresentacion *ventana; /**< la ventana sombrero que contene la aplicacion */
    sombrero::Image *image;
    std::vector<std::string> jpegArchivos; /**< los jpeg archivos que la aplicacion debe MIRAR */
    int corrienteArchivo;

  public:
    Presentacion(std::string camino);
    ~Presentacion();

    void inicializacionSombrero();
    void verNuevo (bool siguiente);
    void correr();
};
  

#endif // __PRESENTACION_H__
