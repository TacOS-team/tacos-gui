#ifndef _GWINDOW_H_
#define _GWINDOW_H_

#include <pronlib.h>

using namespace std;


struct GWindow {
  Window window;
  Window parent;
  Window closeButton;
  GC closeButtonGC;
  Window resizeButton;
  PronWindowAttributes attributes;
  PronWindowAttributes parentAttributes;
  Display *display;

  GWindow (Window w, const PronWindowAttributes & attributes, bool decorate, Display *display);

  bool hasDecoration();
  bool overlaps(GWindow *gw);

  void decorate();
  void resize(int width, int height);
};

#endif// _GWINDOW_H_

