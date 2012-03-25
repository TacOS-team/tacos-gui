#ifndef _SCREEN_H_
#define _SCREEN_H_

#include <vector>
#include <clip_zone.h>
#include <gc.h>
#include <windowsTree.h>

extern "C" {
#include <vesa_types.h>
}

using namespace std;

class Window;

class Screen {

private:
  Screen(int width, int height, int bitsPerPixel);
  static Screen *instance;
  Window *clipWin; /*< Window for which the clipping zone is set */
  ClipZone *clipZone; /*< Rectangles where we are allowed to draw */
  Window *mouseWin; // First window containing the mouse pointer

  void drawHorizLine(int x, int y, int width, bool check = true);
  void drawVertLine(int x, int y, int height, bool check = true);

  bool isValid(int x, int y);

public: // XXX: bourrin
  int width, height;
  int bitsPerPixel;
  int bytesPerPixel;
  int vesa_fd;
  char *videoBuffer;
  vector<Drawable*> drawables; // XXX: ABR ? Rouge/noir ? B-Arbre ?
  WindowsTree *tree;
  GC defaultGC;
  GC *gc;

  static Screen* getInstance(int width, int height, int bitsPerPixel);
  static Screen* getInstance();
  
  bool prepareDrawing(Window *w, GC *gc = NULL);

  void drawPoint(int x, int y, bool check = true);
  void drawLine(int x1, int y1, int x2, int y2, bool check = true);
  void drawRect(int x, int y, int width, int height, bool check = true);
  void drawCircle(int x, int y, int r);

  void fillCircle(int n_cx, int n_cy, int radius);
  void fillRectangle(int x, int y, int width, int height, bool check = true);

  void putImage(PronImage *image, int x, int y);

  Drawable* getDrawable(unsigned int id, int drawableType);
  void addWindow(Window *w);

  Window* getClipWin();
  void setClipWin(Window *w);

  Window* getMouseWin();
  void setMouseWin(Window *mouseWin);

  Window* getRoot();
  void setRoot(Window *root);

  void printClipZone();

  void traceWindows();

  GC* getGC();
  void setGC(GC *gc);

};

#endif
