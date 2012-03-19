#ifndef _GWINDOW_H_
#define _GWINDOW_H_

#include <pronlib.h>

using namespace std;


class GWindow {
 protected:
  bool isMaximised;

 public:
  Window window;
  Window parent;
  Window closeButton;
  GC closeButtonGC;
  Window resizeButton;
  Window maximiseButton;
  PronWindowAttributes attributes;
  PronWindowAttributes parentAttributes;
  PronWindowAttributes oldParentAttributes;
  Display *display;

  GWindow (Window w, const PronWindowAttributes & attributes, bool decorate, Display *display);

  bool hasDecoration();
  bool overlaps(GWindow *gw);

  void decorate();
  void resize(int width, int height);
  void move(int xMove, int yMove);
  void maximise();
  void destroy();
};

#endif// _GWINDOW_H_

