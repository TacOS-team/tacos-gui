#ifndef _SCREEN_H_
#define _SCREEN_H_

#include <vector>
#include <clip_zone.h>

extern "C" {
#include <vesa_types.h>
}

using namespace std;

class Window;

struct GC {
  color_t fg;
};

class Screen {

private:
  Screen(int width, int height, int bitsPerPixel);
  static Screen *instance;
  int mouseX;
  int mouseY;

  void drawHorizLine(int x, int y, int width);
  void drawVertLine(int x, int y, int height);

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
  GC gc;

  static Screen* getInstance(int width, int height, int bitsPerPixel);
  static Screen* getInstance();
  
  void prepareDrawing(Window *w);

  void drawPoint(int x, int y);
  void drawLine(int x1, int y1, int x2, int y2);
  void drawRect(int x, int y, int width, int height) ;
  void drawCircle(int x, int y, int r);

  void fillCircle(int n_cx, int n_cy, int radius);
  void fillRectangle(int x, int y, int width, int height);

  Window* getWindow(int id);
  void addWindow(Window *w);

  void setMouseX(int mouseX);
  int getMouseX();
  void setMouseY(int mouseY);
  int getMouseY();

  void traceWindows();
};

#endif
