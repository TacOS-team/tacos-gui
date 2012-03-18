#ifndef _SCREEN_H_
#define _SCREEN_H_

#include <vector>
#include <clip_zone.h>
#include <gc.h>

extern "C" {
#include <vesa_types.h>
}

using namespace std;

class Window;

class Screen {

private:
  Screen(int width, int height, int bitsPerPixel);
  static Screen *instance;
  Window * mouseWin; // First window containing the mouse pointer

  void drawHorizLine(int x, int y, int width);
  void drawVertLine(int x, int y, int height);

  bool isValid(int x, int y);

public: // XXX: bourrin
  int width, height;
  int bitsPerPixel;
  int bytesPerPixel;
  int vesa_fd;
  char *videoBuffer;
  vector<Window*> windows; // XXX: ABR ? Rouge/noir ? B-Arbre ?
  Window *root;
  Window *clipWin;
  ClipZone *clipZone;
  GC defaultGC;
  GC *gc;

  static Screen* getInstance(int width, int height, int bitsPerPixel);
  static Screen* getInstance();
  
  void setClipWindow(Window *w);
  bool prepareDrawing(Window *w, GC *gc = NULL);

  void drawPoint(int x, int y, bool check = true);
  void drawLine(int x1, int y1, int x2, int y2);
  void drawRect(int x, int y, int width, int height) ;
  void drawCircle(int x, int y, int r);

  void fillCircle(int n_cx, int n_cy, int radius);
  void fillRectangle(int x, int y, int width, int height);

  Window* getWindow(unsigned int id);
  void addWindow(Window *w);

  Window* getMouseWin();
  void setMouseWin(Window *mouseWin);
  Window* getRoot();
  void setRoot(Window *root);

  void traceWindows();

};

#endif
