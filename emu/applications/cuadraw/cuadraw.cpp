#include <stdio.h>
#include <stdlib.h>
#include <sombrero.h>
#include <unistd.h>
#include "cuadraw.h"

class CoolCanvas : public sombrero::Canvas, public has_slots<> {
 private:
 protected:
 public:
  CoolCanvas(int width, int height) : Canvas(width, height) {
    this->subscribeEvent(pron::EV_MOUSE_BUTTON);
    this->subscribeEvent(pron::EV_POINTER_MOVED);
  }
  // Signals
  signal1<sombrero::MouseButton> mouseClicked;

  void handleMouseClicked(sombrero::MouseButton b) {
    this->mouseClicked(b);
  }
};

Cuadraw::Cuadraw(int argc, char **argv) 
    : fileIn(NULL), fileOut(NULL) {
  this->init(argc, argv);
  this->initSombrero();
}

Cuadraw::~Cuadraw() {
  delete this->w;
  if (this->fileIn != NULL) {
    fclose(this->fileIn);
  }
  if (this->fileOut != NULL) {
    fclose(this->fileOut);
  }
}

void Cuadraw::init(int argc, char **argv) {
  int opt;
  while ((opt = getopt(argc, argv, "i:o:")) != -1) {
    switch (opt) {
      case 'i':
        this->fileIn = fopen(optarg, "r");
        if (this->fileIn == NULL) {
          fprintf(stderr, "File given in input not found, starting from sratch.\n");
        }
        break;
      case 'o':
        this->fileOut = fopen(optarg, "w");
        if (this->fileOut == NULL) {
          fprintf(stderr, "Failed to create an output file, aborting.\n");
        }
        break;
      default: /* '?' */
        fprintf(stderr, "Usage: %s [-i fileIn] [-o fileOut]\n", argv[0]);
        exit(EXIT_FAILURE);
    }
  }
  // Traitement des cas ou on a pas de fichier de sortie
  if (this->fileOut == NULL) {
    char *tempNam = tempnam(TEMP_NAM_FOLDER, TEMP_NAM_PREFIX);
    this->fileOut = fopen(tempNam, "w");
    if (this->fileOut == NULL) {
      fprintf(stderr, "Failed to create the default output file %s\n", tempNam);
    } else {
      printf("Writing in a default output file %s\n", tempNam);
    }
    free(tempNam);
  }
}

void Cuadraw::initSombrero() {
  this->w = new sombrero::Window(0, 0, 500, 500);
  this->g = new sombrero::Grid();
  w->add(g);
  this->bp = new sombrero::Button("Pixel");
  this->bp->clicked.connect(this, &Cuadraw::doPixelClicked);
  this->bc = new sombrero::Button("Cirle");
  this->c = new CoolCanvas(500, 500);
  c->mouseClicked.connect(this, &Cuadraw::canvasMouseClicked);
  c->mouseMoved.connect(this, &Cuadraw::canvasMouseMoved);
  g->add(this->bp);
  g->newLine();
  g->add(this->bc);
  g->attach(this->c, 1, 0, 6, 2);
  sombrero::Application::getInstance()->sombrerun();
}

void Cuadraw::doPixelClicked(){
  printf("hello world\n");
}

void Cuadraw::canvasMouseMoved(int xMove, int yMove, int x, int y) {
  printf("mouse Moved at (%d %d) (%d %d)\n", x, y, xMove, yMove);
}

void Cuadraw::canvasMouseClicked(sombrero::MouseButton b) {
  printf("mouse clicked %d\n", b);
}

int main (int argc, char **argv) {
  Cuadraw c(argc, argv);
  return 0;
}
