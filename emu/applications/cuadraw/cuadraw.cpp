#include <stdio.h>
#include <stdlib.h>
#include <sombrero.h>
#include <unistd.h>
#include "cuadraw.h"

int abs(int val) {
  return (val > 0) ? val : -val;
}

class ColorPickerWindow : public sombrero::Window {
 private:
  sombrero::ColorPicker cp;
 protected:
 public:
  sombrero::ColorPicker* getColorPicker() {
    return &cp;
  }
  ColorPickerWindow(std::string title, int x, int y,
      int width, int height, bool decorate = true) 
      : Window(title, x, y, width, height, decorate), cp(30) {
    add(&cp);
  }
  void handleEventDestroyWindow() {}
};

Cuadraw::CoolCanvas::CoolCanvas(int width, int height) : sombrero::Canvas(width, height) {
  this->subscribeEvent(pron::EV_MOUSE_BUTTON);
  this->subscribeEvent(pron::EV_POINTER_MOVED);
}

void Cuadraw::CoolCanvas::handleMouseDown(sombrero::MouseButton b, int x, int y) {
  this->mouseClicked(b, x, y);
}
void Cuadraw::CoolCanvas::handleMouseReleased(sombrero::MouseButton b, int x, int y) {
  this->mouseReleased(b, x, y);
}

void Cuadraw::clearSignals() {
  c->mouseClicked.disconnect_all();
  c->mouseReleased.disconnect_all();
  c->mouseMoved.disconnect_all();
}

Cuadraw::Cuadraw(int argc, char **argv) 
    : fileIn(NULL), fileOut(NULL), wcp(NULL) {
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
  this->mouseDown = false;
  this->w = new sombrero::Window("Cuadraw", 0, 0, 500, 500);
  this->g = new sombrero::Grid();
  this->w->add(g);
  this->bp = new sombrero::Button("Pixel");
  this->bp->clicked.connect(this, &Cuadraw::doPixelClicked);
  this->bl = new sombrero::Button("Line");
  this->bl->clicked.connect(this, &Cuadraw::doLineClicked);
  this->bc = new sombrero::Button("Cirle");
  this->bc->clicked.connect(this, &Cuadraw::doCircleClicked);
  this->bfc = new sombrero::Button("Fill Circle");
  this->bfc->clicked.connect(this, &Cuadraw::doFillCircleClicked);
  this->br = new sombrero::Button("Rect");
  this->br->clicked.connect(this, &Cuadraw::doRectClicked);
  this->bfr = new sombrero::Button("Fill Rect");
  this->bfr->clicked.connect(this, &Cuadraw::doFillRectClicked);
  this->bcp = new sombrero::Button("More Colors");
  this->bcp->clicked.connect(this, &Cuadraw::doColorPickerWindow);
  this->c = new CoolCanvas(500, 500);
  this->cp = new sombrero::ColorPicker(10);
  this->g->add(this->bp);
  this->g->newLine();
  this->g->add(this->bl);
  this->g->newLine();
  this->g->add(this->bc);
  this->g->newLine();
  this->g->add(this->bfc);
  this->g->newLine();
  this->g->add(this->br);
  this->g->newLine();
  this->g->add(this->bfr);
  this->g->newLine();
  this->g->add(this->bcp);
  this->g->attach(this->c, 1, 0, 6, 6);
  this->g->attach(this->cp, 1, 6, 6, 1);
  // Color changed 
  this->cp->colorChanged.connect(this, &Cuadraw::doColorChanged);
  /*this->cp = new sombrero::ColorPicker(10);
  this->g->attach(this->cp, 0, 0, 1, 1);*/
  sombrero::Application::getInstance()->sombrerun();
}

void Cuadraw::doColorPickerWindow() {
  if (this->wcp == NULL) {
    this->wcp = new ColorPickerWindow("ColorPicker", 10, 10, 400, 300);
    this->wcp->getColorPicker()->colorChanged.connect(this, &Cuadraw::doColorChanged);
  }
}

/**
 * !!! Drawing modes !!!
 */

void Cuadraw::doColorChanged(Color c) {
  this->c->setFGColor(c);
  this->cp->setCurrentColor(c);
  if (this->wcp != NULL) {
    this->wcp->getColorPicker()->setCurrentColor(c);
  }
}

void Cuadraw::doPixelClicked(){
  this->clearSignals();
  c->mouseClicked.connect(this, &Cuadraw::pixelMouseClicked);
  c->mouseReleased.connect(this, &Cuadraw::pixelMouseReleased);
  c->mouseMoved.connect(this, &Cuadraw::pixelMouseMoved);
}

void Cuadraw::pixelMouseMoved(int xMove __attribute__((unused)), int yMove __attribute__((unused)), int x, int y) {
  if (this->mouseDown) {
    this->c->drawPoint(x, y);
    this->c->draw();
  }
}

void Cuadraw::pixelMouseClicked(sombrero::MouseButton b __attribute__((unused)), int x __attribute__((unused)), int y __attribute__((unused))) {
  this->mouseDown = true;
}

void Cuadraw::pixelMouseReleased(sombrero::MouseButton b __attribute__((unused)), int x, int y) {
  this->mouseDown = false;
  this->c->switchPixmap();
  this->downX = x;
  this->downY = y;
}

void Cuadraw::doLineClicked(){
  this->clearSignals();
  c->mouseClicked.connect(this, &Cuadraw::lineMouseClicked);
  c->mouseReleased.connect(this, &Cuadraw::lineMouseReleased);
  c->mouseMoved.connect(this, &Cuadraw::lineMouseMoved);
}

void Cuadraw::lineMouseMoved(int xMove __attribute__((unused)), int yMove __attribute__((unused)), int x, int y) {
  if (this->mouseDown) {
    this->c->restorePixmap();
    this->c->drawLine(this->downX, this->downY, x, y);
    this->c->draw();
  } else {
    this->downX = x;
    this->downY = y;
  }
}

void Cuadraw::lineMouseClicked(sombrero::MouseButton b __attribute__((unused)), int x __attribute__((unused)), int y __attribute__((unused))) {
  this->mouseDown = true;
}

void Cuadraw::lineMouseReleased(sombrero::MouseButton b __attribute__((unused)), int x, int y) {
  this->mouseDown = false;
  this->c->switchPixmap();
  this->downX = x;
  this->downY = y;
}

void Cuadraw::doCircleClicked(){
  this->clearSignals();
  c->mouseClicked.connect(this, &Cuadraw::circleMouseClicked);
  c->mouseReleased.connect(this, &Cuadraw::circleMouseReleased);
  c->mouseMoved.connect(this, &Cuadraw::circleMouseMoved);
}

void Cuadraw::circleMouseMoved(int xMove __attribute__((unused)), int yMove __attribute__((unused)), int x, int y) {
  if (this->mouseDown) {
    this->c->restorePixmap();
    this->c->drawEllipse(this->downX + (x - this->downX) / 2, this->downY + (y - this->downY) / 2, abs((x - this->downX) / 2), abs((y - this->downY) / 2));
    this->c->draw();
  } else {
    this->downX = x;
    this->downY = y;
  }
}

void Cuadraw::circleMouseClicked(sombrero::MouseButton b __attribute__((unused)), int x __attribute__((unused)), int y __attribute__((unused))) {
  this->mouseDown = true;
}

void Cuadraw::circleMouseReleased(sombrero::MouseButton b __attribute__((unused)), int x, int y) {
  this->mouseDown = false;
  this->c->switchPixmap();
  this->downX = x;
  this->downY = y;
}

void Cuadraw::doFillCircleClicked(){
  this->clearSignals();
  c->mouseClicked.connect(this, &Cuadraw::fillCircleMouseClicked);
  c->mouseReleased.connect(this, &Cuadraw::fillCircleMouseReleased);
  c->mouseMoved.connect(this, &Cuadraw::fillCircleMouseMoved);
}

void Cuadraw::fillCircleMouseMoved(int xMove __attribute__((unused)), int yMove __attribute__((unused)), int x, int y) {
  if (this->mouseDown) {
    this->c->restorePixmap();
    this->c->fillEllipse(this->downX + (x - this->downX) / 2, this->downY + (y - this->downY) / 2, abs((x - this->downX) / 2), abs((y - this->downY) / 2));
    this->c->draw();
  } else {
    this->downX = x;
    this->downY = y;
  }
}

void Cuadraw::fillCircleMouseClicked(sombrero::MouseButton b __attribute__((unused)), int x __attribute__((unused)), int y __attribute__((unused))) {
  this->mouseDown = true;
}

void Cuadraw::fillCircleMouseReleased(sombrero::MouseButton b __attribute__((unused)), int x, int y) {
  this->mouseDown = false;
  this->c->switchPixmap();
  this->downX = x;
  this->downY = y;
}

void Cuadraw::doRectClicked(){
  this->clearSignals();
  c->mouseClicked.connect(this, &Cuadraw::rectMouseClicked);
  c->mouseReleased.connect(this, &Cuadraw::rectMouseReleased);
  c->mouseMoved.connect(this, &Cuadraw::rectMouseMoved);
}

void Cuadraw::rectMouseMoved(int xMove __attribute__((unused)), int yMove __attribute__((unused)), int x, int y) {
  if (this->mouseDown) {
    this->c->restorePixmap();
    this->c->drawRectangle((this->downX > x) ? x : this->downX , (this->downY > y) ? y : this->downY, abs(this->downX - x), abs(this->downY - y));
    this->c->draw();
  } else {
    this->downX = x;
    this->downY = y;
  }
}

void Cuadraw::rectMouseClicked(sombrero::MouseButton b __attribute__((unused)), int x __attribute__((unused)), int y __attribute__((unused))) {
  this->mouseDown = true;
}

void Cuadraw::rectMouseReleased(sombrero::MouseButton b __attribute__((unused)), int x, int y) {
  this->mouseDown = false;
  this->c->switchPixmap();
  this->downX = x;
  this->downY = y;
}

void Cuadraw::doFillRectClicked(){
  this->clearSignals();
  c->mouseClicked.connect(this, &Cuadraw::fillRectMouseClicked);
  c->mouseReleased.connect(this, &Cuadraw::fillRectMouseReleased);
  c->mouseMoved.connect(this, &Cuadraw::fillRectMouseMoved);
}

void Cuadraw::fillRectMouseMoved(int xMove __attribute__((unused)), int yMove __attribute__((unused)), int x, int y) {
  if (this->mouseDown) {
    this->c->restorePixmap();
    this->c->fillRectangle((this->downX > x) ? x : this->downX , (this->downY > y) ? y : this->downY, abs(this->downX - x), abs(this->downY - y));
    this->c->draw();
  } else {
    this->downX = x;
    this->downY = y;
  }
}

void Cuadraw::fillRectMouseClicked(sombrero::MouseButton b __attribute__((unused)), int x __attribute__((unused)), int y __attribute__((unused))) {
  this->mouseDown = true;
}

void Cuadraw::fillRectMouseReleased(sombrero::MouseButton b __attribute__((unused)), int x, int y) {
  this->mouseDown = false;
  this->c->switchPixmap();
  this->downX = x;
  this->downY = y;
}

int main (int argc, char **argv) {
  Cuadraw c(argc, argv);
  return 0;
}

