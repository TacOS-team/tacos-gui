#ifndef _GWINDOW_H_
#define _GWINDOW_H_

#include <pronlib.h>

using namespace std;


struct GWindow {
  Window window;
  Window parent;
  GC closeButtonGC;
  GC resizeButtonGC;
  PronWindowAttributes attributes;
  PronWindowAttributes parentAttributes;
  Display *display;

  GWindow (Window w, const PronWindowAttributes & attributes, bool decorate, Display *display);

  bool hasDecoration();
  bool overlaps(GWindow *gw);

  void decorate();

  bool isCloseButton (int x, int y);
  bool isResizeButton(int x, int y);
};

#endif// _GWINDOW_H_

