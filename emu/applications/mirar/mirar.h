#ifndef __MIRAR_H__
#define __MIRAR_H__

#include <sombrero.h>
#include <scrollpane.h>

class Mirar :public has_slots<> {
  private:
    sombrero::Window *w;
    sombrero::Grid *g;
    sombrero::Button *next;
    sombrero::Button *prev;
    sombrero::Image *image;
    sombrero::ScrollPane *sp;
    char * path;

  public:
    Mirar(char * path);
    ~Mirar();

    void initSombrero(char * path);
    void run();

};
  



#endif // __MIRAR_H__
