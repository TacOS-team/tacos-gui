#ifndef _GWINDOW_H_
#define _GWINDOW_H_

#include <pronlib.h>

using namespace std;


struct GWindow {
  Window window;
  Window parent;
  Window closeButton;
  Window resizeButton;
  PronWindowAttributes attributes;
  PronWindowAttributes parentAttributes;

  GWindow (Window w, Window parent);

  bool hasDecoration();
  bool overlaps(GWindow *gw);
};

#endif// _GWINDOW_H_

