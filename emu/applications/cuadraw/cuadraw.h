#ifndef __CUADRAW_H__
#define __CUADRAW_H_
#include <sombrero.h>

#define TEMP_NAM_PREFIX "cuad"
#define TEMP_NAM_FOLDER "/tmp"

class Cuadraw {
 private:
  FILE *fileIn; /**< The input file */
  FILE *fileOut; /**< The output file */
  sombrero::Window *w; /**< The main window */
  sombrero::Canvas *c; /**< The canvas of the where to draw */
  sombrero::Grid *g; /**< The sombrero grid */
  sombrero::Button *bp; /**< Pixel brush button */
  sombrero::Button *bc; /**< Circle brush button */
  /**
   * Initialize cuadraw
   * @param argc
   * @param argv
   */
  void init(int argc, char **argv);
  /**
   * Initialize sombrero stuff
   */
  void initSombrero();
 protected:
 public:
  /**
   * Cuadraw constructor
   * @param argc
   * @param argv
   */
  Cuadraw(int argc, char **argv);
  /**
   * Lanches the cuadraw application
   */
  void run();
  /**
   * Cuadraw destructor
   */
  ~Cuadraw();
};
#endif//__CUADRAW_H__
