#ifndef _GWINDOW_H_
#define _GWINDOW_H_

#include <pronlib.h>

using namespace std;


struct GWindow {
  Window window;
  Window parent;
  int x;
  int y;
  unsigned int width;
  unsigned int height;

  GWindow (Window w, Window parent);

  bool hasDecoration();
};

#endif// _GWINDOW_H_

