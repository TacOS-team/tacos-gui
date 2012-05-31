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
  signal1<sombrero::MouseButton> mouseReleased;
  
  void handleMouseClicked(sombrero::MouseButton b) {
    this->mouseClicked(b);
  }
  void handleMouseReleased(sombrero::MouseButton b) {
    this->mouseReleased(b);
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
  this->downX = 0;
  this->downY = 0;
  this->w = new sombrero::Window(0, 0, 500, 500);
  this->g = new sombrero::Grid();
  w->add(g);
  this->bp = new sombrero::Button("Pixel");
  this->bp->clicked.connect(this, &Cuadraw::doPixelClicked);
  this->bc = new sombrero::Button("Cirle");
  this->bc->clicked.connect(this, &Cuadraw::doCircleClicked);
  this->c = new CoolCanvas(500, 500);
  g->add(this->bp);
  g->newLine();
  g->add(this->bc);
  g->attach(this->c, 1, 0, 6, 2);
  sombrero::Application::getInstance()->sombrerun();
}

/**
 * !!! Drawing modes !!!
 */

void Cuadraw::doPixelClicked(){
  c->mouseClicked.disconnect_all();
  c->mouseReleased.disconnect_all();
  c->mouseMoved.disconnect_all();
  c->mouseClicked.connect(this, &Cuadraw::pixelMouseClicked);
  c->mouseReleased.connect(this, &Cuadraw::pixelMouseReleased);
  c->mouseMoved.connect(this, &Cuadraw::pixelMouseMoved);
}

void Cuadraw::pixelMouseMoved(int xMove, int yMove, int x, int y) {
  printf("Pixel : mouse Moved at (%d %d) (%d %d)\n", x, y, xMove, yMove);
  if (this->mouseDown) {
    this->c->drawPoint(x, y);
    this->c->draw();
  } else {
    this->downX = x;
    this->downY = y;
  }
}

void Cuadraw::pixelMouseClicked(sombrero::MouseButton b) {
  this->mouseDown = true;
  printf("Pixel : mouse clicked %d (%d)\n", b, this->mouseDown);
}

void Cuadraw::pixelMouseReleased(sombrero::MouseButton b) {
  this->mouseDown = false;
  this->c->switchPixmap();
  printf("Pixel : mouse released %d (%d)\n", b, this->mouseDown);
}

void Cuadraw::doCircleClicked(){
  c->mouseClicked.disconnect_all();
  c->mouseReleased.disconnect_all();
  c->mouseMoved.disconnect_all();
  c->mouseClicked.connect(this, &Cuadraw::circleMouseClicked);
  c->mouseReleased.connect(this, &Cuadraw::circleMouseReleased);
  c->mouseMoved.connect(this, &Cuadraw::circleMouseMoved);
  // Updates the last mouse down coordinates
}

int abs(int val) {
  return (val > 0) ? val : -val;
}

void Cuadraw::circleMouseMoved(int xMove, int yMove, int x, int y) {
  printf("Circle : mouse Moved at (%d %d) (%d %d)\n", x, y, xMove, yMove);
  if (this->mouseDown) {
    this->c->restorePixmap();
    printf("Last coord (%d, %d)\n", this->downX, this->downY);
    this->c->drawEllipse(this->downX + (x - this->downX) / 2, this->downY + (y - this->downY) / 2, abs((x - this->downX) / 2), abs((y - this->downY) / 2));
    this->c->draw();
  } else {
    this->downX = x;
    this->downY = y;
  }
}

void Cuadraw::circleMouseClicked(sombrero::MouseButton b) {
  this->mouseDown = true;
  printf("Circle : mouse clicked %d (%d)\n", b, this->mouseDown);
}

void Cuadraw::circleMouseReleased(sombrero::MouseButton b) {
  this->mouseDown = false;
  this->c->switchPixmap();
  printf("Circle : mouse released %d (%d)\n", b, this->mouseDown);
}


int main (int argc, char **argv) {
  Cuadraw c(argc, argv);
  return 0;
}

